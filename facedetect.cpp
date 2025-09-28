#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <cstdlib>
#include "voice_command.h"

using namespace cv;

// Global variables for voice control
bool face_detection_enabled = true;
bool show_fps = false;
bool voice_commands_enabled = false;
std::string openai_api_key;

void toggleFaceDetection() {
    face_detection_enabled = !face_detection_enabled;
    std::cout << "Face detection " << (face_detection_enabled ? "enabled" : "disabled") << std::endl;
}

void toggleFPS() {
    show_fps = !show_fps;
    std::cout << "FPS display " << (show_fps ? "enabled" : "disabled") << std::endl;
}

void exitApplication() {
    std::cout << "Exiting application..." << std::endl;
    exit(0);
}

void takeScreenshot() {
    std::cout << "Screenshot command received!" << std::endl;
    // Screenshot functionality can be implemented here
}

int main() {
    // Get OpenAI API key from environment variable
    const char* api_key_env = std::getenv("OPENAI_API_KEY");
    if (api_key_env) {
        openai_api_key = api_key_env;
        voice_commands_enabled = true;
    } else {
        std::cout << "OpenAI API key not found. Voice commands disabled." << std::endl;
        std::cout << "Set OPENAI_API_KEY environment variable to enable voice commands." << std::endl;
    }

    // Initialize voice commands
    VoiceCommandProcessor voice_processor;
    if (voice_commands_enabled && voice_processor.initialize(openai_api_key)) {
        // Register voice commands
        voice_processor.registerCommand("start detection", toggleFaceDetection);
        voice_processor.registerCommand("stop detection", toggleFaceDetection);
        voice_processor.registerCommand("toggle detection", toggleFaceDetection);
        voice_processor.registerCommand("show fps", toggleFPS);
        voice_processor.registerCommand("hide fps", toggleFPS);
        voice_processor.registerCommand("exit", exitApplication);
        voice_processor.registerCommand("quit", exitApplication);
        voice_processor.registerCommand("close", exitApplication);
        voice_processor.registerCommand("screenshot", takeScreenshot);
        voice_processor.registerCommand("capture", takeScreenshot);
        
        voice_processor.startListening();
        std::cout << "Voice commands enabled! Try saying:" << std::endl;
        std::cout << "  - 'start detection' / 'stop detection'" << std::endl;
        std::cout << "  - 'show fps' / 'hide fps'" << std::endl;
        std::cout << "  - 'screenshot' / 'capture'" << std::endl;
        std::cout << "  - 'exit' / 'quit'" << std::endl;
    }

    // Load face cascade
    CascadeClassifier face_cascade;
    face_cascade.load("C:/Users/rajes/source/repos/opencv/data/haarcascades/haarcascade_frontalface_default.xml");

    VideoCapture cap(0); // Open webcam
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open webcam" << std::endl;
        return -1;
    }

    Mat frame, gray;
    auto start_time = std::chrono::high_resolution_clock::now();
    int frame_count = 0;
    
    std::cout << "Face detection started. Press ESC to quit." << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Convert to grayscale for face detection
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Perform face detection only if enabled
        if (face_detection_enabled) {
            std::vector<Rect> faces;
            face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));

            // Draw rectangles around detected faces
            for (const auto& face : faces) {
                rectangle(frame, face, Scalar(0, 255, 255), 2); // Cyan rectangles
            }

            // Display face count
            putText(frame, "Faces: " + std::to_string(faces.size()), 
                   Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
        } else {
            // Show "Detection Disabled" message
            putText(frame, "Detection Disabled", Point(10, 30), 
                   FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
        }

        // Calculate and display FPS if enabled
        if (show_fps) {
            frame_count++;
            auto current_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(
                current_time - start_time).count();
            
            if (duration > 0) {
                double fps = frame_count / static_cast<double>(duration);
                putText(frame, "FPS: " + std::to_string(static_cast<int>(fps)), 
                       Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 0, 0), 2);
            }
        }

        // Display voice command status
        if (voice_commands_enabled) {
            putText(frame, "Voice Commands: ON", Point(10, frame.rows - 10), 
                   FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
        }

        imshow("Face Detection with Voice Commands", frame);
        
        int key = waitKey(30) & 0xFF;
        if (key == 27) break; // ESC to quit
        
        // Keyboard shortcuts
        if (key == 'd' || key == 'D') toggleFaceDetection();
        if (key == 'f' || key == 'F') toggleFPS();
        if (key == 's' || key == 'S') takeScreenshot();
    }

    if (voice_commands_enabled) {
        voice_processor.stopListening();
    }

    return 0;
}

