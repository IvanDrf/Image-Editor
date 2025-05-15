#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <string>
#include <vector>

class Image;
class FileField;
class StatusBar;
class Brush;

using Paths = std::vector<std::string>;
using StackImage = std::stack<sf::Image>;

// The main function that handles pressing each button
void WorkWithPath(Paths& pathToFile, size_t activeFile, const std::string& result, size_t buttonNumber, Image& image, FileField& FileField, StatusBar& StatusBar, bool& brushPressed,
                  StackImage& previousStatus);
//

// Functions that are responsible for uploading files, etc. backend
namespace Back {
void AddFile(Paths& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, StackImage& previousStatus);
void DeleteFile(Paths& pathToFile, size_t activeFile, const std::string& result, Image& image, FileField& FileField, StatusBar& statusBar, StackImage& previousStatus, bool& brushPressed);

void SaveFile(const std::string& result, Image& image, StatusBar& statusBar);
void SelectFile(Paths& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, StackImage& previousStatus);
void SelectBrush(bool& brushPressed, const Image& image, StatusBar& StatusBar);

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
