 # ![Title](https://github.com/user-attachments/assets/816e617d-a344-450c-9c1c-19d0a596ad70)

# 🎨 SFML Image Editor

*A simple yet powerful image editing tool built with C++ and SFML*

## Table of Contents
- [Features](#-features)
- [Installation](#-installation)
- [Quick Start](#-quick-start)
- [User Guide](#-user-guide)
- [Development](#-development)
- [Contributing](#-contributing)
- [License](#-license)

## ✨ Features

- 🖼️ **Image Loading**: Supports PNG, JPG, BMP formats
- 🎨 **Painting Tools**:
  - Adjustable brush size
  - Custom color selection (RGB)
- 💾 **Save Functionality**: Export as PNG/JPG
- 🗑️ **File Management**: Add/remove images
- 🖱️ **Intuitive UI**: Easy-to-use controls

## 📥 Installation

### Requirements
- C++17 compiler (GCC, Clang, or MSVC)
- SFML 2.5.1
- CMake 3.20

### Build Instructions
```bash
# Clone repository
git clone https://github.com/yourusername/sfml-image-editor.git
cd sfml-image-editor

# Build with CMake
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Run application (Linux/Mac)
./ImageEditor

# Windows
.\Release\ImageEditor.exe
