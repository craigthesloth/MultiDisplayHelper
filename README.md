# MultiDisplayHelper

A Qt-based tool for capturing and controlling multiple screens remotely. This application allows you to view and interact with secondary displays from your primary monitor.

## Features

- **Screen Capture**: Real-time capture of secondary displays with adjustable FPS
- **Remote Mouse Control**: Full mouse control over captured screens (clicks, movement, scrolling)
- **Scaled Display**: Adaptive scaling of captured screens with visual feedback
- **Performance Monitoring**: Real-time FPS display and performance metrics
- **Fullscreen Mode**: Toggle between windowed and fullscreen viewing
- **Cross-Platform**: Currently supports Windows with Qt framework

## Requirements

- Qt 5 or Qt 6
- C++17 compatible compiler
- CMake 3.16 or higher
- Windows (for mouse control functionality)

## Building

### Using CMake

```bash
# Create build directory
mkdir build
cd build

# Configure project
cmake ..

# Build project
cmake --build .

## Usage
Select Target Screen: Choose the secondary display from the dropdown list
Set Capture FPS: Adjust frames per second (1-60) for performance/quality balance
Start Capture: Click "Start Capture" to begin screen streaming
Remote Control:
      Click anywhere on the captured screen to move the mouse
      Use mouse buttons for left/right/middle clicks
Fullscreen: Toggle fullscreen mode for better viewing

## Project Structure
MultiDisplayHelper/
├── CMakeLists.txt          # Build configuration
├── main.cpp               # Application entry point
├── mainwindow.h/cpp       # Main application window
├── mainwindow.ui          # UI layout file
├── screen_capturer.h/cpp  # Screen capture functionality
├── mouse_controller.h/cpp # Remote mouse control
└── screen_widget.h/cpp    # Display widget with scaling


## Version
Version 1.02 BETA

## Author
P.Sobin
