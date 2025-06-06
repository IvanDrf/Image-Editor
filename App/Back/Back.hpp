#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <string>
#include <vector>

#include "../MainWindow/MainWindow.hpp"

class Image;
class FileField;
class StatusBar;
class Brush;

struct AppData {
    Paths& pathsToFile;         // Vector with paths of files
    size_t& previousFile;       // Previous active file
    size_t& activeFile;         // Current active file
    const std::string& result;  // Buttons function result

    Image& image;        // Current image
    float& currentZoom;  // Current zoom

    FileField& fileField;
    StatusBar& statusBar;
    StackImage& previousStatus;

    Brush& brush;
    bool& brushPressed;
};

namespace Back {
// The main function that handles pressing each button
void ButtonsFunc(AppData& data, size_t buttonNumber);
//

// Functions that are responsible for uploading files, etc. backend

void AddFile(AppData& data);
void DeleteFile(AppData& data);

void SaveFile(AppData& data);
void SelectBrush(AppData& data);

void Reset(AppData& data);
}  // namespace Back

// Functions that process the path
namespace Path {
void DeletePath(Paths& pathsToFile, const std::string& fileName);
[[nodiscard]] std::string FindPath(Paths& pathsToFile, const std::string& fileName);
[[nodiscard]] std::string GetFileName(const std::string& fileName);

}  // namespace Path

namespace Zoom {
void ZoomIn(Image& image);
void ZoomOut(Image& image);

void Reset(Image& image, StatusBar& statusBar, Brush& brush, float& currentZoom);
}  // namespace Zoom
