#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

int main() {
    std::cout << "🎯 SIMPLE FACE DETECTION" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "📹 Starting camera..." << std::endl;

    // Load the face cascade
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load("haarcascade_frontalface_alt.xml")) {
        std::cerr << "❌ Error loading face cascade" << std::endl;
        return -1;
    }

    // Open the default camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "❌ Cannot open camera" << std::endl;
        return -1;
    }

    std::cout << "✅ Camera opened successfully!" << std::endl;
    std::cout << "🎮 Controls:" << std::endl;
    std::cout << "   ESC or Q - Quit" << std::endl;
    std::cout << "   D - Toggle face detection" << std::endl;
    std::cout << "   F - Toggle FPS display" << std::endl;
    std::cout << "   S - Save screenshot" << std::endl << std::endl;

    bool face_detection_enabled = true;
    bool show_fps = false;
    int frame_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "❌ Failed to capture frame" << std::endl;
            break;
        }

        // Convert to grayscale for face detection
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        // Detect faces if enabled
        std::vector<cv::Rect> faces;
        if (face_detection_enabled) {
            face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

            // Draw rectangles around detected faces
            for (const auto& face : faces) {
                cv::Point center(face.x + face.width / 2, face.y + face.height / 2);
                cv::ellipse(frame, center, cv::Size(face.width / 2, face.height / 2), 
                           0, 0, 360, cv::Scalar(255, 0, 255), 4);
            }

            // Display face count
            std::string face_text = "Faces: " + std::to_string(faces.size());
            cv::putText(frame, face_text, cv::Point(10, 30), 
                       cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        } else {
            cv::putText(frame, "Face detection OFF", cv::Point(10, 30), 
                       cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }

        // Calculate and display FPS if enabled
        if (show_fps) {
            frame_count++;
            if (frame_count % 30 == 0) {  // Update every 30 frames
                auto current_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
                float fps = 30000.0f / duration.count();
                
                std::string fps_text = "FPS: " + std::to_string((int)fps);
                cv::putText(frame, fps_text, cv::Point(10, 70), 
                           cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 0), 2);
                
                start_time = current_time;
            }
        }

        // Display status
        std::string status = face_detection_enabled ? "Detection: ON" : "Detection: OFF";
        cv::putText(frame, status, cv::Point(frame.cols - 200, 30), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

        // Show the frame
        cv::imshow("Simple Face Detection", frame);

        // Handle keyboard input
        char key = cv::waitKey(1);
        if (key == 27 || key == 'q' || key == 'Q') {  // ESC or Q
            std::cout << "🚪 Exiting application..." << std::endl;
            break;
        } else if (key == 'd' || key == 'D') {
            face_detection_enabled = !face_detection_enabled;
            std::cout << "Face detection " << (face_detection_enabled ? "enabled" : "disabled") << std::endl;
        } else if (key == 'f' || key == 'F') {
            show_fps = !show_fps;
            std::cout << "FPS display " << (show_fps ? "enabled" : "disabled") << std::endl;
        } else if (key == 's' || key == 'S') {
            std::string filename = "screenshot_" + std::to_string(std::time(nullptr)) + ".jpg";
            cv::imwrite(filename, frame);
            std::cout << "📸 Screenshot saved as " << filename << std::endl;
        }
    }

    cv::destroyAllWindows();
    std::cout << "✅ Application closed successfully" << std::endl;
    return 0;
}