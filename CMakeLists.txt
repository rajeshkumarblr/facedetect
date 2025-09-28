cmake_minimum_required(VERSION 3.10)
project(facedetect)

set(OpenCV_ROOT "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
find_package(OpenCV REQUIRED CONFIG)

add_executable(facedetect facedetect.cpp)
target_link_libraries(facedetect ${OpenCV_LIBS})