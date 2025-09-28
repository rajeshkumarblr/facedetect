# Face Detection with OpenCV

A real-time face detection application using OpenCV and CMake.

## Features
- Real-time face detection from webcam
- **🎙️ Voice Commands** - Control the app with your voice using OpenAI Whisper
- Uses Haar Cascade classifier
- Built with OpenCV 4.x and CMake
- Cross-platform support
- FPS display toggle
- Detection enable/disable
- Voice-activated screenshot capture

## Prerequisites
- **CMake 3.10+**
- **vcpkg** (Visual Studio 2022 built-in version recommended)
- **Visual Studio 2022** or compatible C++ compiler
- **Webcam** for real-time detection
- **Microphone** for voice commands (optional)
- **OpenAI API Key** for voice commands (optional)

## Installation Instructions

### 1. Install vcpkg

#### Option A: Visual Studio 2022 (Recommended - Easiest)
- Install Visual Studio 2022 Community (free)
- During installation, check "Desktop development with C++"
- vcpkg is included automatically

#### Option B: Standalone vcpkg
```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap
.\bootstrap-vcpkg.bat  # Windows
./bootstrap-vcpkg.sh   # Linux/macOS

# Integrate with system
.\vcpkg integrate install
```

### 2. Clone This Project
```bash
git clone https://github.com/rajeshkumarblr/facedetect.git
cd facedetect
```

### 3. Build the Project

#### Option A: Using VS Code (Recommended)
1. Open the project in VS Code
2. Install the **CMake Tools** extension
3. Press `Ctrl+Shift+P` → "CMake: Configure"
4. Press `Ctrl+Shift+B` to build

#### Option B: Using Command Line

**With Visual Studio 2022:**
```powershell
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:/Program Files/Microsoft Visual Studio/2022/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build
```

**With standalone vcpkg:**
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="/path/to/your/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build
```

### 4. Setup OpenAI API Key (Optional - For Voice Commands)
To enable voice commands, you need an OpenAI API key:

1. **Get API Key:**
   - Go to [OpenAI API Keys](https://platform.openai.com/api-keys)
   - Create an account and generate an API key

2. **Set Environment Variable:**
   ```powershell
   # Windows (PowerShell)
   $env:OPENAI_API_KEY="your-api-key-here"
   
   # Windows (Command Prompt)
   set OPENAI_API_KEY=your-api-key-here
   
   # Linux/macOS
   export OPENAI_API_KEY="your-api-key-here"
   ```

3. **For Permanent Setup (Windows):**
   - Press `Win + R`, type `sysdm.cpl`
   - Click "Environment Variables"
   - Add `OPENAI_API_KEY` with your API key value

### 5. Download Haar Cascade File
Download the face detection model from OpenCV:
1. Go to [OpenCV Haar Cascades](https://github.com/opencv/opencv/blob/master/data/haarcascades/haarcascade_frontalface_default.xml)
2. Save as: `C:/Users/YOUR_USERNAME/source/repos/opencv/data/haarcascades/haarcascade_frontalface_default.xml`
3. Or update the path in `facedetect.cpp` to point to your downloaded file

### 6. Run the Application
```powershell
.\build\Debug\facedetect.exe
```

## Usage

### Basic Controls
- The application will open your default webcam
- Detected faces are highlighted with cyan rectangles
- Press **ESC** to quit the application

### Keyboard Shortcuts
- **D** - Toggle face detection on/off
- **F** - Toggle FPS display
- **S** - Take screenshot (placeholder)

### 🎙️ Voice Commands (if enabled)
The app listens for these voice commands:

| Command | Action |
|---------|--------|
| "start detection" / "stop detection" | Toggle face detection |
| "toggle detection" | Toggle face detection |
| "show fps" / "hide fps" | Toggle FPS display |
| "screenshot" / "capture" | Take screenshot |
| "exit" / "quit" / "close" | Exit application |

**Note:** Voice commands require an OpenAI API key and microphone access.

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

### ❌ CMake can't find OpenCV
Make sure you're using the vcpkg toolchain file:
```bash
-DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

### ❌ vcpkg not installed
Install Visual Studio 2022 Community (includes vcpkg) or install standalone vcpkg as shown above.

### ❌ Webcam not detected
- Ensure your webcam is not being used by another application
- Try changing the camera index in `facedetect.cpp` from `0` to `1` or `2`

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

### ❌ Voice commands not working
- **Check API key:** Make sure `OPENAI_API_KEY` environment variable is set
- **Check microphone:** Ensure your microphone is working and not used by other apps
- **Check internet:** Voice commands require internet connection for OpenAI API
- **Speak clearly:** Wait for "Listening for command..." message, then speak clearly
- **Check console:** Look for error messages in the console output

### ❌ "No default input device found"
- Make sure you have a working microphone connected
- Check Windows sound settings to ensure microphone is enabled
- Try running the application as administrator

## Dependencies
The project automatically installs these dependencies via vcpkg:
- **OpenCV 4.x** - Computer vision and face detection
- **CURL** - HTTP requests to OpenAI API
- **nlohmann-json** - JSON parsing for API responses  
- **PortAudio** - Audio recording for voice commands

## Project Structure
```
facedetect/
├── facedetect.cpp          # Main application code
├── voice_command.h         # Voice command header
├── voice_command.cpp       # Voice command implementation
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