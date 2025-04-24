#include "MainWindow.hpp"

#include <limits>

#include "../Brush/Brush.hpp"
#include "../FileField/FileField.hpp"
#include "../Image/Image.hpp"
#include "../StatusBar/StatusBar.hpp"

// Main Methods for working with files
void WorkWithPath(Paths& pathToFile, size_t activeFile, const std::string& result, size_t buttonNumber, Image& image, FileField& fileField, StatusBar& statusBar, bool& brushPressed,
                  StackImage& previousStatus) {
    switch (static_cast<Buttons>(buttonNumber)) {
        // Add file button
        case (Buttons::AddFile): {
            Back::AddFile(pathToFile, result, image, fileField, statusBar, previousStatus);

            break;
        }

        // Delete file button
        case (Buttons::DeleteFile): {
            Back::DeleteFile(pathToFile, activeFile, result, image, fileField, statusBar, previousStatus, brushPressed);

            break;
        }

        // Save file button
        case (Buttons::SaveFile): {
            Back::SaveFile(result, image, statusBar);

            break;
        }

        case (Buttons::SelectBrush): {
            Back::SelectBrush(brushPressed, image, statusBar);

            break;
        }
    }
}

namespace Back {
void AddFile(Paths& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, StackImage& previousStatus) {
    if (result.empty()) {
        return;
    }

    try {
        if (std::find(fileField.GetFiles().begin(), fileField.GetFiles().end(), Path::GetFileName(result)) != fileField.GetFiles().end()) {
            statusBar.UpdateStatus("File has already been added");

            return;
        }

        sf::Image temp;
        if (!temp.loadFromFile(result)) {
            throw std::invalid_argument("Image could not be found");
        }

        pathToFile.push_back(result);      // Add new path to the new file
        image.ClearImage(previousStatus);  // Clear if there was some image
        image.LoadImage(result);           // Load new image
        image.SetMainImageScale();

        fileField.AddFile(Path::GetFileName(result));

        statusBar.UpdateStatus("Image loaded successfully", sf::Color::Green);
    } catch (...) {
        statusBar.UpdateStatus("Image could not be found", sf::Color::Red);
    }
}

void SaveFile(const std::string& result, Image& image, StatusBar& statusBar) {
    if (!result.empty() && image.SaveImage(result)) {
        statusBar.UpdateStatus("File was saved successfully", sf::Color::Green);

        return;
    }

    statusBar.UpdateStatus("Unable to save the file", sf::Color::Red);
}

void DeleteFile(Paths& pathToFile, size_t activeFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, StackImage& previousStatus, bool& brushPressed) {
    if (result.empty()) {
        return;
    }

    size_t oldLength{fileField.GetFiles().size()};  // Old count of files
    fileField.DeleteFile(Path::GetFileName(result));
    size_t newLength{fileField.GetFiles().size()};  // New count of files

    if (oldLength == newLength) {
        statusBar.UpdateStatus("File not found", sf::Color::Red);

        return;
    }

    statusBar.UpdateStatus("File was deleted successfully", sf::Color::Green);
    Path::DeletePath(pathToFile, result);  // Delete path to deleting file

    image.ClearImage(previousStatus);  // Reset scale, texture and e.t.c
    if (!pathToFile.empty()) {
        image.LoadImage(pathToFile[activeFile]);  // Load previous image
        image.SetMainImageScale();
    } else {
        brushPressed = false;  // If there no files left
    }
}

void SelectFile(Paths& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, StackImage& previousStatus) {
    if (!result.empty() && std::find(fileField.GetFiles().begin(), fileField.GetFiles().end(), result) != fileField.GetFiles().end()) {
        statusBar.UpdateStatus("File " + result + " selected", sf::Color::Green);

        image.ClearImage(previousStatus);                     // Clear old image
        image.LoadImage(Path::FindPath(pathToFile, result));  // Select image
        image.SetMainImageScale();

        return;
    }

    statusBar.UpdateStatus("File " + result + " cannot be selected", sf::Color::Red);
}

void SelectBrush(bool& brushPressed, const Image& image, StatusBar& statusBar) {
    brushPressed = !brushPressed && image.HasImage();
    if (brushPressed) {
        statusBar.UpdateStatus("Brush selected", sf::Color::Green);
        return;
    }

    statusBar.UpdateStatus("Brush is no longer selected");
}

#ifndef NONE
#define NONE (std::numeric_limits<size_t>::max())
void SelectNewActiveFile(size_t buttonNumber, size_t& activeFile, size_t files) {
    if (buttonNumber == Buttons::AddFile) {
        activeFile = files - 1;
        return;
    }

    if (buttonNumber == Buttons::DeleteFile) {
        --activeFile;
        if (activeFile == NONE && files > 0) {
            activeFile = 0;
        }
    }
}
#endif

}  // namespace Back

namespace Path {
void DeletePath(Paths& pathToFile, const std::string& fileName) {
    for (const auto& file : pathToFile) {
        if (file == fileName) {
            pathToFile.erase(std::remove(pathToFile.begin(), pathToFile.end(), file), pathToFile.end());
        }
    }
}

std::string FindPath(Paths& pathToFile, const std::string& fileName) {
    for (const auto& file : pathToFile) {
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
}  // namespace Zoom
