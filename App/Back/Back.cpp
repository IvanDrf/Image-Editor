#include "Back.hpp"

#include <limits>

#include "../Brush/Brush.hpp"
#include "../FileField/FileField.hpp"
#include "../Image/Image.hpp"
#include "../StatusBar/StatusBar.hpp"

// Main Methods for working with files
namespace Back {
void ButtonsFunc(AppData& data, size_t buttonNumber) {
    switch (static_cast<Buttons>(buttonNumber)) {
        // Add file button
        case (Buttons::AddFile): {
            Back::AddFile(data);

            break;
        }

        // Delete file button
        case (Buttons::DeleteFile): {
            Back::DeleteFile(data);

            break;
        }

        // Save file button
        case (Buttons::SaveFile): {
            Back::SaveFile(data);

            break;
        }

        // Select Brush
        case (Buttons::SelectBrush): {
            Back::SelectBrush(data);

            break;
        }

        case (Buttons::Reset): {
            Back::Reset(data);
        }

        default: {
            return;
        }
    }
}

void AddFile(AppData& data) {
    if (data.result.empty()) {
        return;
    }

    try {
        if (std::find(data.fileField.GetFiles().begin(), data.fileField.GetFiles().end(), Path::GetFileName(data.result)) != data.fileField.GetFiles().end()) {
            data.statusBar.UpdateStatus("File has already been added");

            return;
        }

        sf::Image temp;
        if (!temp.loadFromFile(data.result)) {
            throw std::invalid_argument("Image could not be found");
        }

        data.pathsToFile.push_back(data.result);     // Add new path to the new file
        data.image.ClearImage(data.previousStatus);  // Clear if there was some image
        data.image.LoadImage(data.result);           // Load new image
        data.image.SetMainImageScale();

        data.fileField.AddFile(Path::GetFileName(data.result));

        data.statusBar.UpdateStatus("Image loaded successfully", sf::Color::Green);

        ActiveFile::ChangeActiveFileNumber(Buttons::AddFile, data.activeFile, data.pathsToFile.size());
        data.currentZoom = kDefaultZoom;                 // Reset zoom
        data.brush.UpdateCursorScale(data.currentZoom);  // Reset zoom

    } catch (...) {
        data.statusBar.UpdateStatus("Image could not be found", sf::Color::Red);
    }
}

void SaveFile(AppData& data) {
    if (!data.result.empty() && data.image.SaveImage(data.result)) {
        data.statusBar.UpdateStatus("File was saved successfully", sf::Color::Green);

        return;
    }

    data.statusBar.UpdateStatus("Unable to save the file", sf::Color::Red);
}

void DeleteFile(AppData& data) {
    if (data.result.empty()) {
        return;
    }

    data.fileField.DeleteFile(Path::GetFileName(data.result));
    data.statusBar.UpdateStatus("File was deleted successfully", sf::Color::Green);
    Path::DeletePath(data.pathsToFile, data.result);  // Delete path to deleting file

    ActiveFile::ChangeActiveFileNumber(Buttons::DeleteFile, data.activeFile, data.pathsToFile.size());
    data.currentZoom = kDefaultZoom;                 // Reset zoom
    data.brush.UpdateCursorScale(data.currentZoom);  // Reset zoom

    data.image.ClearImage(data.previousStatus);  // Reset scale, texture and e.t.c
    if (!data.pathsToFile.empty()) {
        data.image.LoadImage(data.pathsToFile[data.activeFile]);  // Load previous image
        data.image.SetMainImageScale();

    } else {
        data.brushPressed = false;  // If there no files left
    }
}

void SelectFile(AppData& data) {
    if (!data.result.empty() && std::find(data.fileField.GetFiles().begin(), data.fileField.GetFiles().end(), data.result) != data.fileField.GetFiles().end()) {
        data.statusBar.UpdateStatus("File " + data.result + " selected", sf::Color::Green);

        data.image.ClearImage(data.previousStatus);                           // Clear old image
        data.image.LoadImage(Path::FindPath(data.pathsToFile, data.result));  // Select image
        data.image.SetMainImageScale();

        return;
    }

    data.statusBar.UpdateStatus("File " + data.result + " cannot be selected", sf::Color::Red);
}

void SelectBrush(AppData& data) {
    data.brushPressed = !data.brushPressed && data.image.HasImage();
    if (data.brushPressed) {
        data.statusBar.UpdateStatus("Brush selected", sf::Color::Green);
        return;
    }

    data.statusBar.UpdateStatus("Brush is no longer selected");
}

void Reset(AppData& data) {
    if (data.image.HasImage()) {
        Zoom::Reset(data.image, data.statusBar, data.brush, data.currentZoom);
    }
}
}  // namespace Back

namespace Path {
void DeletePath(Paths& pathsToFile, const std::string& fileName) {
    for (const auto& file : pathsToFile) {
        if (file == fileName) {
            pathsToFile.erase(std::remove(pathsToFile.begin(), pathsToFile.end(), file), pathsToFile.end());
        }
    }
}

std::string FindPath(Paths& pathsToFile, const std::string& fileName) {
    for (const auto& file : pathsToFile) {
        if (GetFileName(file) == fileName) {
            return file;
        }
    }

    return "";
}

// Get file name from the path smth/dir/name
std::string GetFileName(const std::string& fileName) {
    std::string reversedName{};
    for (size_t i = fileName.length() - 1; i < fileName.length() && fileName[i] != '/'; --i) {
        reversedName += fileName[i];
    }

    std::string result{};
    for (size_t i = reversedName.length() - 1; i >= 0 && i < reversedName.length(); --i) {
        result += reversedName[i];
    }

    return result;
}
}  // namespace Path

namespace Zoom {
void ZoomIn(Image& image) {
    image.SetScale(image.GetScale().x + kZoomStep, image.GetScale().y + kZoomStep);
    image.SetPosition(kFileFieldWidth, kButtonHeight);
}

void ZoomOut(Image& image) {
    image.SetScale(image.GetScale().x - kZoomStep, image.GetScale().y - kZoomStep);
    image.SetPosition(kFileFieldWidth, kButtonHeight);
}

void Reset(Image& image, StatusBar& statusBar, Brush& brush, float& currentZoom) {
    image.SetPosition(kFileFieldWidth, kButtonHeight);
    image.SetMainImageScale();

    currentZoom = kDefaultZoom;

    brush.UpdateCursorScale(currentZoom);
    statusBar.UpdateStatus("Current zoom: " + std::to_string(currentZoom).substr(0, 3));
}
}  // namespace Zoom
