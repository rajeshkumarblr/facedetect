cmake_minimum_required(VERSION 3.10)
project(facedetect)

find_package(OpenCV REQUIRED)

add_executable(facedetect facedetect.cpp)
target_link_libraries(facedetect ${OpenCV_LIBS})
set_property(TARGET facedetect PROPERTY CXX_STANDARD 17)
