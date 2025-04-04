#include "MainWindow.hpp"

#include "../Brush/Brush.hpp"
#include "../FileField/FileField.hpp"
#include "../Image/Image.hpp"
#include "../StatusBar/StatusBar.hpp"

// Load Icons for buttons
std::vector<sf::RectangleShape> LoadButtonImages() {
    sf::Texture addIcon;
    if (!addIcon.loadFromFile("../WindowFiles/add-image.png")) {
        throw std::runtime_error("Add Image could not be uploaded");
    }

    sf::Texture deleteIcon;
    if (!deleteIcon.loadFromFile("../WindowFiles/delete-image.png")) {
        throw std::runtime_error("Delete image could not be uploaded");
    }

    sf::Texture saveIcon;
    if (!saveIcon.loadFromFile("../WindowFiles/save-image.png")) {
        throw std::runtime_error("Save image could not be uploaded");
    }

    sf::Texture selectIcon;
    if (!selectIcon.loadFromFile("../WindowFiles/select-image.png")) {
        throw std::runtime_error("Select image could not be uploaded");
    }

    sf::Texture brushIcon;
    if (!brushIcon.loadFromFile("../WindowFiles/brush-image.png")) {
        throw std::runtime_error("Brush image could not be uploaded");
    }

    static const std::vector<sf::Texture> icons = {addIcon, deleteIcon, saveIcon, selectIcon, brushIcon};
    std::vector<sf::RectangleShape> iconShapes;  // Icon shapes and icons

    for (size_t i = 0; i < icons.size(); ++i) {
        iconShapes.emplace_back(sf::Vector2f(kButtonWidth / 5.5f, kButtonWidth / 5.5f));
        iconShapes.back().setOrigin(0, iconShapes.back().getGlobalBounds().height / 2);
        iconShapes.back().setPosition(kSmallMenuWidth + kIconX + i * kButtonWidth, kButtonHeight / 2);

        iconShapes.back().setTexture(&icons[i]);
    }

    iconShapes[0].setPosition(kSmallMenuWidth + kIconX * 1.5f, kButtonHeight / 2);

    return iconShapes;
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

// Main Methods for working with files
void ReleaseFunctions(const std::string& result, size_t buttonNumber, Image& image, FileField& fileField, StatusBar& statusBar, bool& brushPressed, std::stack<sf::Image>& previousStatus) {
    static std::vector<std::string> pathToFile;  // Paths to images

    switch (static_cast<Buttons>(buttonNumber)) {
        // Add file button
        case (Buttons::AddFile): {
            Back::AddFile(pathToFile, result, image, fileField, statusBar, previousStatus);

            break;
        }

        // Delete file button
        case (Buttons::DeleteFile): {
            Back::DeleteFile(pathToFile, result, image, fileField, statusBar, previousStatus);

            break;
        }

        // Save file button
        case (Buttons::SaveFile): {
            Back::SaveFile(result, image, statusBar);

            break;
        }

        // Select File button
        case (Buttons::SelectFile): {
            Back::SelectFile(pathToFile, result, image, fileField, statusBar, previousStatus);

            break;
        }

        case (Buttons::SelectBrush): {
            Back::SelectBrush(brushPressed, image, statusBar);

            break;
        }
    }
}

namespace Back {
void AddFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus) {
    if (result.empty()) {
        return;
    }

    try {
        if (std::find(fileField.GetFiles().begin(), fileField.GetFiles().end(), GetFileName(result)) != fileField.GetFiles().end()) {
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

        fileField.AddFile(GetFileName(result));

        statusBar.UpdateStatus("Image loaded successfully", sf::Color::Green);
    } catch (std::exception& e) {
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

void DeleteFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus) {
    if (result.empty()) {
        return;
    }

    size_t oldLength{fileField.GetFiles().size()};  // Old count of files
    fileField.DeleteFile(GetFileName(result));
    size_t newLength{fileField.GetFiles().size()};  // New count of files

    if (oldLength == newLength) {
        statusBar.UpdateStatus("File not found", sf::Color::Red);

        return;
    }

    statusBar.UpdateStatus("File was deleted successfully", sf::Color::Green);

    image.ClearImage(previousStatus);
    Back::DeletePath(pathToFile, result);  // Delete path to deleting file
}

void SelectFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus) {
    if (!result.empty() && std::find(fileField.GetFiles().begin(), fileField.GetFiles().end(), result) != fileField.GetFiles().end()) {
        statusBar.UpdateStatus("File " + result + " selected", sf::Color::Green);

        image.ClearImage(previousStatus);                     // Clear old image
        image.LoadImage(Back::FindPath(pathToFile, result));  // Select image
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

void DeletePath(std::vector<std::string>& pathToFile, const std::string& fileName) {
    for (const auto& file : pathToFile) {
        if (GetFileName(file) == fileName) {
            pathToFile.erase(std::remove(pathToFile.begin(), pathToFile.end(), file), pathToFile.end());
        }
    }
}

std::string FindPath(std::vector<std::string>& pathToFile, const std::string& fileName) {
    for (const auto& file : pathToFile) {
        if (GetFileName(file) == fileName) {
            return file;
        }
    }

    return "";
}
}  // namespace Back
