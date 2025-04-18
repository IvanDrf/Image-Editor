 # ![Title](https://github.com/user-attachments/assets/816e617d-a344-450c-9c1c-19d0a596ad70)

# 🎨 SFML Image Editor

*A simple yet powerful image editing tool built with C++ and SFML*

## Table of Contents
- [Features](#-features)
- [Installation](#-installation)
- [User Guide](#-user-guide)
  - [Add File](#-add-file)
  - [Delete File](#-delete-file)
  - [Save File](#-save-file)
  - [Brush](#-brush)
- [Shortcut](#-shortcut)

## ✨ Features

- 🖼️ **Image Loading**: Supports PNG, JPG formats
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
cmake .. && cmake --build .
cd .. && ./ImageEditor
```

## 🖱️ User Guide

### 🆕 Add File
https://github.com/user-attachments/assets/665d8c56-911c-4e1f-b6d1-1931e4fcb9a5

<br/><br/>

### 🗑️ Delete File
https://github.com/user-attachments/assets/5bc3680b-8053-4490-b5d4-f49481d5db10

<br/><br/>

### 💾 Save File
https://github.com/user-attachments/assets/7618526e-6380-4027-af88-33b5589c4b8a

<br/><br/>

### 🖌️ Brush
https://github.com/user-attachments/assets/b6bf4a59-6226-4203-839e-261ee8f306b2

<br/><br/>

## ⌨️ Shortcut

### 📄 File
| Action  | Shortcut    | Description              |
|---------|-------------|--------------------------|
| Add     | `Ctrl + N`  | Add new image            |
| Delete  | `Del`       | Delete selected image    |
| Save    | `Ctrl + S`  | Save current image       |
| Up      | `↑`         | Move selection up        |
| Down    | `↓`         | Move selection down      |

### 🎨 Brush
| Action       | Shortcut | Description                  |
|--------------|----------|------------------------------|
| Red          | `R`      | Set brush color to red       |
| Green        | `G`      | Set brush color to green     |
| Blue         | `B`      | Set brush color to blue      |
| Yellow       | `Y`      | Set brush color to yellow    |
| White        | `W`      | Set brush color to white     |
| Size Down    | `[`      | Decrease brush size          |
| Size Up      | `]`      | Increase brush size          |




