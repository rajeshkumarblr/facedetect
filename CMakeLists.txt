cmake_minimum_required(VERSION 3.10)
project(facedetect)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

# Add executable with all source files
add_executable(facedetect 
    facedetect.cpp 
    voice_command.cpp
)

# Link libraries
target_link_libraries(facedetect 
    ${OpenCV_LIBS}
    CURL::libcurl
    nlohmann_json::nlohmann_json
    ${PORTAUDIO_LIBRARIES}
)

# Include directories
target_include_directories(facedetect PRIVATE ${PORTAUDIO_INCLUDE_DIRS})

# Compiler definitions
target_compile_definitions(facedetect PRIVATE ${PORTAUDIO_CFLAGS_OTHER})

# Set C++ standard
set_property(TARGET facedetect PROPERTY CXX_STANDARD 17)