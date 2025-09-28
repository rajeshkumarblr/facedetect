#include "voice_command.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <mutex>

using json = nlohmann::json;

// Callback for CURL to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

VoiceCommandProcessor::VoiceCommandProcessor() 
    : listening_(false), recording_(false), stream_(nullptr) {
}

VoiceCommandProcessor::~VoiceCommandProcessor() {
    stopListening();
    cleanupAudio();
}

bool VoiceCommandProcessor::initialize(const std::string& openai_api_key) {
    api_key_ = openai_api_key;
    
    if (api_key_.empty()) {
        std::cerr << "Error: OpenAI API key not provided" << std::endl;
        return false;
    }
    
    // Initialize CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Initialize PortAudio
    return initializeAudio();
}

bool VoiceCommandProcessor::initializeAudio() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    
    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        std::cerr << "Error: No default input device found" << std::endl;
        return false;
    }
    
    inputParameters.channelCount = CHANNELS;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
    
    err = Pa_OpenStream(&stream_,
                        &inputParameters,
                        nullptr,  // no output
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paClipOff,
                        audioCallback,
                        this);
    
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    
    return true;
}

void VoiceCommandProcessor::cleanupAudio() {
    if (stream_) {
        Pa_CloseStream(stream_);
        stream_ = nullptr;
    }
    Pa_Terminate();
    curl_global_cleanup();
}

int VoiceCommandProcessor::audioCallback(const void* inputBuffer, void* outputBuffer,
                                       unsigned long framesPerBuffer,
                                       const PaStreamCallbackTimeInfo* timeInfo,
                                       PaStreamCallbackFlags statusFlags,
                                       void* userData) {
    VoiceCommandProcessor* processor = static_cast<VoiceCommandProcessor*>(userData);
    const float* input = static_cast<const float*>(inputBuffer);
    
    if (processor->recording_) {
        std::lock_guard<std::mutex> lock(processor->buffer_mutex_);
        processor->audio_buffer_.insert(processor->audio_buffer_.end(), 
                                      input, input + framesPerBuffer);
    }
    
    return paContinue;
}

void VoiceCommandProcessor::startListening() {
    if (listening_) return;
    
    listening_ = true;
    
    PaError err = Pa_StartStream(stream_);
    if (err != paNoError) {
        std::cerr << "PortAudio error starting stream: " << Pa_GetErrorText(err) << std::endl;
        listening_ = false;
        return;
    }
    
    // Start processing thread
    processing_thread_ = std::thread(&VoiceCommandProcessor::processAudioData, this);
    
    std::cout << "Voice command listening started. Say commands..." << std::endl;
}

void VoiceCommandProcessor::stopListening() {
    if (!listening_) return;
    
    listening_ = false;
    recording_ = false;
    
    if (stream_) {
        Pa_StopStream(stream_);
    }
    
    if (processing_thread_.joinable()) {
        processing_thread_.join();
    }
    
    std::cout << "Voice command listening stopped." << std::endl;
}

void VoiceCommandProcessor::registerCommand(const std::string& command, std::function<void()> callback) {
    commands_[command] = callback;
}

void VoiceCommandProcessor::processAudioData() {
    while (listening_) {
        // Start recording
        recording_ = true;
        audio_buffer_.clear();
        
        std::cout << "Listening for command... (speak now)" << std::endl;
        
        // Record for specified duration
        std::this_thread::sleep_for(std::chrono::seconds(RECORD_SECONDS));
        
        // Stop recording
        recording_ = false;
        
        // Process the recorded audio
        std::vector<float> recorded_audio;
        {
            std::lock_guard<std::mutex> lock(buffer_mutex_);
            recorded_audio = audio_buffer_;
        }
        
        if (!recorded_audio.empty()) {
            std::string transcription = transcribeAudio(recorded_audio);
            if (!transcription.empty()) {
                processCommand(transcription);
            }
        }
        
        // Wait before next recording cycle
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

std::string VoiceCommandProcessor::transcribeAudio(const std::vector<float>& audioData) {
    // Convert float audio data to WAV format for OpenAI API
    std::string wav_data = makeWhisperAPIRequest(audioData);
    return wav_data;
}

std::string VoiceCommandProcessor::makeWhisperAPIRequest(const std::vector<float>& audioData) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    if (!curl) {
        return "";
    }
    
    // Create a temporary WAV file
    std::string temp_filename = "temp_audio.wav";
    
    // Simple WAV header creation
    std::ofstream wav_file(temp_filename, std::ios::binary);
    if (!wav_file) {
        curl_easy_cleanup(curl);
        return "";
    }
    
    // WAV header
    const char* riff = "RIFF";
    wav_file.write(riff, 4);
    
    uint32_t file_size = 36 + audioData.size() * sizeof(float);
    wav_file.write(reinterpret_cast<const char*>(&file_size), 4);
    
    const char* wave = "WAVE";
    wav_file.write(wave, 4);
    
    const char* fmt = "fmt ";
    wav_file.write(fmt, 4);
    
    uint32_t fmt_size = 16;
    wav_file.write(reinterpret_cast<const char*>(&fmt_size), 4);
    
    uint16_t audio_format = 3; // IEEE float
    wav_file.write(reinterpret_cast<const char*>(&audio_format), 2);
    
    uint16_t num_channels = CHANNELS;
    wav_file.write(reinterpret_cast<const char*>(&num_channels), 2);
    
    uint32_t sample_rate = SAMPLE_RATE;
    wav_file.write(reinterpret_cast<const char*>(&sample_rate), 4);
    
    uint32_t byte_rate = SAMPLE_RATE * CHANNELS * sizeof(float);
    wav_file.write(reinterpret_cast<const char*>(&byte_rate), 4);
    
    uint16_t block_align = CHANNELS * sizeof(float);
    wav_file.write(reinterpret_cast<const char*>(&block_align), 2);
    
    uint16_t bits_per_sample = 32;
    wav_file.write(reinterpret_cast<const char*>(&bits_per_sample), 2);
    
    const char* data = "data";
    wav_file.write(data, 4);
    
    uint32_t data_size = audioData.size() * sizeof(float);
    wav_file.write(reinterpret_cast<const char*>(&data_size), 4);
    
    // Write audio data
    wav_file.write(reinterpret_cast<const char*>(audioData.data()), data_size);
    wav_file.close();
    
    // Setup CURL for OpenAI API request
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/audio/transcriptions");
    
    // Headers
    struct curl_slist* headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + api_key_;
    headers = curl_slist_append(headers, auth_header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    // Form data
    curl_mime* mime = curl_mime_init(curl);
    curl_mimepart* part;
    
    // File part
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "file");
    curl_mime_filedata(part, temp_filename.c_str());
    
    // Model part
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "model");
    curl_mime_data(part, "whisper-1", CURL_ZERO_TERMINATED);
    
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
    // Perform request
    res = curl_easy_perform(curl);
    
    // Cleanup
    curl_mime_free(mime);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    // Remove temp file
    std::remove(temp_filename.c_str());
    
    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        return "";
    }
    
    // Parse JSON response
    try {
        json response = json::parse(readBuffer);
        if (response.contains("text")) {
            return response["text"];
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
    
    return "";
}

void VoiceCommandProcessor::processCommand(const std::string& transcription) {
    std::cout << "Heard: \"" << transcription << "\"" << std::endl;
    
    // Convert to lowercase for case-insensitive matching
    std::string lower_transcription = transcription;
    std::transform(lower_transcription.begin(), lower_transcription.end(), 
                   lower_transcription.begin(), ::tolower);
    
    // Check for registered commands
    for (const auto& [command, callback] : commands_) {
        std::string lower_command = command;
        std::transform(lower_command.begin(), lower_command.end(), 
                       lower_command.begin(), ::tolower);
        
        if (lower_transcription.find(lower_command) != std::string::npos) {
            std::cout << "Executing command: " << command << std::endl;
            callback();
            return;
        }
    }
    
    std::cout << "No matching command found for: " << transcription << std::endl;
}