# Face Detection with OpenCV

A real-time face detection application using OpenCV and CMake.

## Features
- Real-time face detection from webcam
- Uses Haar Cascade classifier
- Built with OpenCV 4.x and CMake
- Cross-platform support

## Prerequisites
- **CMake 3.10+**
- **Visual Studio 2022** (or compatible C++ compiler)
- **vcpkg** (Visual Studio's built-in version is fine)
- **Webcam** for real-time detection

## Dependencies
- **OpenCV 4.x** (automatically installed via vcpkg)

## Build Instructions

### 1. Clone the Repository
```bash
git clone https://github.com/rajeshkumarblr/facedetect.git
cd facedetect
```

### 2. Install Dependencies
The project uses vcpkg manifest mode. Dependencies will be automatically installed during CMake configuration.

### 3. Configure and Build

#### Option A: Using VS Code (Recommended)
1. Open the project in VS Code
2. Install the **CMake Tools** extension
3. Press `Ctrl+Shift+P` → "CMake: Configure"
4. Press `Ctrl+Shift+B` to build

#### Option B: Using Command Line
```powershell
# Configure (Windows with Visual Studio 2022)
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:/Program Files/Microsoft Visual Studio/2022/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Build
cmake --build build
```

### 4. Download Haar Cascade File
Download the face detection model from OpenCV:
1. Go to [OpenCV Haar Cascades](https://github.com/opencv/opencv/blob/master/data/haarcascades/haarcascade_frontalface_default.xml)
2. Save as: `C:/Users/YOUR_USERNAME/source/repos/opencv/data/haarcascades/haarcascade_frontalface_default.xml`
3. Or update the path in `facedetect.cpp` to point to your downloaded file

### 5. Run the Application
```powershell
.\build\Debug\facedetect.exe
```

## Usage
- The application will open your default webcam
- Detected faces are highlighted with cyan rectangles
- Press **ESC** to quit the application

## VS Code Integration
The project includes VS Code configuration files:
- `.vscode/settings.json` - CMake and vcpkg configuration
- `.vscode/launch.json` - Debug configuration
- `.vscode/tasks.json` - Build tasks

### VS Code Shortcuts:
- `Ctrl+F5` - Run without debugging
- `F5` - Run with debugging  
- `Ctrl+Shift+B` - Build project

## Troubleshooting

### CMake can't find OpenCV
Make sure you're using the vcpkg toolchain file:
```bash
-DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

### Webcam not detected
- Ensure your webcam is not being used by another application
- Try changing the camera index in `facedetect.cpp` from `0` to `1` or `2`

### Build errors
- Make sure Visual Studio 2022 is installed with C++ development tools
- Ensure CMake is in your system PATH

## Project Structure
```
facedetect/
├── facedetect.cpp          # Main application code
├── CMakeLists.txt          # CMake configuration
├── vcpkg.json             # vcpkg dependencies
├── .gitignore             # Git ignore rules
├── .vscode/               # VS Code configuration
│   ├── settings.json      # CMake settings
│   ├── launch.json        # Debug configuration
│   └── tasks.json         # Build tasks
└── README.md              # This file
```

## License
This project is open source and available under the MIT License.