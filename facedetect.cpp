#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
using namespace cv;

int main() {
    CascadeClassifier face_cascade;
    face_cascade.load("C:/Users/rajes/source/repos/opencv/data/haarcascades/haarcascade_frontalface_default.xml");

    VideoCapture cap(0); // Open webcam
    if (!cap.isOpened()) return -1;

    Mat frame, gray;
    while (true) {
        cap >> frame;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        std::vector<Rect> faces;
        face_cascade.detectMultiScale(gray, faces);

        for (auto &face : faces) {
            rectangle(frame, face, Scalar(255, 255, 0), 2);
        }

        imshow("Face Detection", frame);
        if (waitKey(30) == 27) break; // ESC to quit
    }
    return 0;
}

