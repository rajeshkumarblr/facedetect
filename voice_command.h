#pragma once

#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <portaudio.h>

class VoiceCommandProcessor {
public:
    VoiceCommandProcessor();
    ~VoiceCommandProcessor();
    
    // Initialize the voice processor
    bool initialize(const std::string& openai_api_key);
    
    // Start/stop listening for voice commands
    void startListening();
    void stopListening();
    
    // Register command callbacks
    void registerCommand(const std::string& command, std::function<void()> callback);
    
    // Check if currently listening
    bool isListening() const { return listening_; }

private:
    // Audio recording
    bool initializeAudio();
    void cleanupAudio();
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);
    
    // Voice processing
    void processAudioData();
    std::string transcribeAudio(const std::vector<float>& audioData);
    void processCommand(const std::string& transcription);
    
    // HTTP request to OpenAI Whisper API
    std::string makeWhisperAPIRequest(const std::vector<float>& audioData);
    
    // Member variables
    std::string api_key_;
    std::atomic<bool> listening_;
    std::atomic<bool> recording_;
    
    // Audio system
    PaStream* stream_;
    std::vector<float> audio_buffer_;
    std::mutex buffer_mutex_;
    
    // Command processing
    std::map<std::string, std::function<void()>> commands_;
    std::thread processing_thread_;
    
    // Audio parameters
    static constexpr int SAMPLE_RATE = 16000;
    static constexpr int FRAMES_PER_BUFFER = 1024;
    static constexpr int CHANNELS = 1;
    static constexpr int RECORD_SECONDS = 3;  // Record for 3 seconds per command
};