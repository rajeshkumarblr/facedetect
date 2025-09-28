# Face Detection with OpenCV

A real-time face detection application using OpenCV and CMake.

## Features
- Real-time face detection from webcam
- Uses Haar Cascade classifier
- Built with OpenCV 4.x and CMake
- Cross-platform support

## Prerequisites
- **CMake 3.10+**
- **Visual Studio 2022** (or compatible C++ compiler with vcpkg) OR standalone vcpkg
- **Webcam** for real-time detection

## Installation Methods

Choose ONE of the following methods to install OpenCV:

## Method 1: Visual Studio 2022 with Built-in vcpkg (Recommended)

### Prerequisites
- Visual Studio 2022 with C++ development tools
- CMake (comes with Visual Studio or install separately)

### 1. Clone the Repository
```bash
git clone https://github.com/rajeshkumarblr/facedetect.git
cd facedetect
```

### 2. Dependencies are Auto-Installed
The project uses vcpkg manifest mode. OpenCV will be automatically installed during CMake configuration.

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

---

## Method 2: Standalone vcpkg Installation

If you don't have Visual Studio 2022 or prefer a standalone vcpkg installation:

### 1. Install vcpkg
```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap (Windows)
.\bootstrap-vcpkg.bat

# Bootstrap (Linux/macOS)
./bootstrap-vcpkg.sh

# Integrate with system
.\vcpkg integrate install  # Windows
./vcpkg integrate install  # Linux/macOS
```

### 2. Clone This Project
```bash
git clone https://github.com/rajeshkumarblr/facedetect.git
cd facedetect
```

### 3. Configure with Custom vcpkg Path
```bash
# Replace /path/to/vcpkg with your actual vcpkg installation path
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build
```

---

## Method 3: Pre-installed OpenCV (System Package)

If you have OpenCV already installed on your system:

### 1. Remove vcpkg Configuration
Delete or rename the `vcpkg.json` file:
```bash
mv vcpkg.json vcpkg.json.bak
```

### 2. Configure without vcpkg
```bash
cmake -B build -S .
cmake --build build
```

### 3. Set OpenCV Path (if needed)
If CMake can't find OpenCV, set the path:
```bash
cmake -B build -S . -DOpenCV_DIR="/path/to/opencv/lib/cmake/opencv4"
```

---

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

### ❌ "vcpkg not found" or "No vcpkg installation"

**Solution 1 (Recommended)**: Install Visual Studio 2022
- Download Visual Studio 2022 Community (free)
- During installation, check "Desktop development with C++"
- This includes built-in vcpkg support

**Solution 2**: Install standalone vcpkg (see Method 2 above)

**Solution 3**: Use system OpenCV (see Method 3 above)

### ❌ CMake can't find OpenCV

**If using vcpkg**: Make sure you're using the toolchain file:
```bash
-DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

**If using system OpenCV**: Try specifying the OpenCV path:
```bash
cmake -B build -S . -DOpenCV_DIR="/usr/local/lib/cmake/opencv4"
```

**On Ubuntu/Debian**:
```bash
sudo apt update
sudo apt install libopencv-dev
```

**On macOS with Homebrew**:
```bash
brew install opencv
```

### ❌ Webcam not detected
- Ensure your webcam is not being used by another application
- Try changing the camera index in `facedetect.cpp` from `0` to `1` or `2`
- On Linux, make sure you have camera permissions

### ❌ Build errors
- Make sure Visual Studio 2022 is installed with C++ development tools
- Ensure CMake is in your system PATH
- Try deleting the `build/` directory and reconfiguring

### ❌ "Haar cascade file not found"
Download the file manually:
1. Go to [OpenCV Haar Cascades](https://github.com/opencv/opencv/blob/master/data/haarcascades/haarcascade_frontalface_default.xml)
2. Save it to your project directory
3. Update the path in `facedetect.cpp`:
   ```cpp
   face_cascade.load("haarcascade_frontalface_default.xml");  // relative path
   ```

## Alternative Files for Different Setups

### For Users Without vcpkg:

1. **Replace CMakeLists.txt**:
   ```bash
   mv CMakeLists.txt CMakeLists.vcpkg.txt
   mv CMakeLists.robust.txt CMakeLists.txt
   ```

2. **Replace VS Code settings**:
   ```bash
   mv .vscode/settings.json .vscode/settings.vcpkg.json
   mv .vscode/settings.no-vcpkg.json .vscode/settings.json
   ```

3. **Remove vcpkg manifest**:
   ```bash
   mv vcpkg.json vcpkg.json.bak
   ```

Now your project will work with system-installed OpenCV!

## Project Structure
```
facedetect/
├── facedetect.cpp              # Main application code
├── CMakeLists.txt              # CMake configuration (vcpkg version)
├── CMakeLists.robust.txt       # Alternative CMake (system OpenCV)
├── vcpkg.json                  # vcpkg dependencies
├── .gitignore                  # Git ignore rules
├── .vscode/                    # VS Code configuration
│   ├── settings.json           # CMake settings (vcpkg version)
│   ├── settings.no-vcpkg.json  # Alternative settings (system OpenCV)
│   ├── launch.json             # Debug configuration
│   └── tasks.json              # Build tasks
└── README.md                   # This file
```

## License
This project is open source and available under the MIT License.