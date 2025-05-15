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
    Paths& pathToFile;          // Vector with paths of files
    size_t& activeFile;         // Current active file
    const std::string& result;  // Buttons function result

    Image& image;  // Current image
    FileField& fileField;
    StatusBar& statusBar;
    StackImage& previousStatus;
    bool& brushPressed;
};

// The main function that handles pressing each button
void ButtonsFunc(AppData& data, size_t buttonNumber);
//

// Functions that are responsible for uploading files, etc. backend
namespace Back {
void AddFile(AppData& data);
void DeleteFile(AppData& data);

void SaveFile(AppData& data);
void SelectFile(AppData& data);
void SelectBrush(AppData& data);

void SelectNewActiveFile(size_t buttonNumber, size_t& activeFile, size_t files);
}  // namespace Back

// Functions that process the path
namespace Path {
void DeletePath(Paths& pathToFile, const std::string& fileName);
[[nodiscard]] std::string FindPath(Paths& pathToFile, const std::string& fileName);
[[nodiscard]] std::string GetFileName(const std::string& fileName);

}  // namespace Path

namespace Zoom {
void ZoomIn(Image& image);
void ZoomOut(Image& image);

void Reset(Image& image, StatusBar& statusBar, Brush& brush, float& currentZoom);
}  // namespace Zoom
