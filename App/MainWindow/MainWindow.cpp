#include "MainWindow.hpp"

#include "../Brush/Brush.hpp"
#include "../Button/Button.hpp"
#include "../FileField/FileField.hpp"
#include "../Image/Image.hpp"
#include "../InputField/InputField.hpp"
#include "../MainMethods/MainMethods.hpp"
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

    sf::Texture brushIcon;
    if (!brushIcon.loadFromFile("../WindowFiles/brush-image.png")) {
        throw std::runtime_error("Brush image could not be uploaded");
    }

    static const std::vector<sf::Texture> icons = {addIcon, deleteIcon, saveIcon, brushIcon};
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
void ReleaseFunctions(std::vector<std::string>& pathToFile, const std::string& result, size_t buttonNumber, Image& image, FileField& fileField, StatusBar& statusBar, bool& brushPressed,
                      std::stack<sf::Image>& previousStatus) {
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

        case (Buttons::SelectBrush): {
            Back::SelectBrush(brushPressed, image, statusBar);

            break;
        }
    }
}
