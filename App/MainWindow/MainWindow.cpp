#include "MainWindow.hpp"

#include <iostream>

#include "../FileField/FileField.hpp"
#include "../Image/Image.hpp"
#include "../StatusBar/StatusBar.hpp"

void LoadButtonImages(std::vector<sf::Sprite>& buttonIcons) {
    static sf::Texture addIcon;
    if (!addIcon.loadFromFile("../WindowFiles/add-image.png")) {
        throw std::runtime_error("Add Image could not be uploaded");
    }

    static sf::Texture deleteIcon;
    if (!deleteIcon.loadFromFile("../WindowFiles/delete-image.png")) {
        throw std::runtime_error("Delete image could not be uploaded");
    }

    static sf::Texture saveIcon;
    if (!saveIcon.loadFromFile("../WindowFiles/save-image.png")) {
        throw std::runtime_error("Save image could not be uploaded");
    }

    sf::Sprite addSprite;
    addSprite.setTexture(addIcon);
    addSprite.setPosition(kIconX + kIconX / 2, kIconY);

    sf::Sprite deleteSprite;
    deleteSprite.setTexture(deleteIcon);
    deleteSprite.setPosition(kButtonWidth + kIconX, kIconY);

    sf::Sprite saveSprite;
    saveSprite.setTexture(saveIcon);
    saveSprite.setPosition(2 * (kButtonWidth + kIconX), kIconY);

    buttonIcons.emplace_back(addSprite);
    buttonIcons.emplace_back(deleteSprite);
    buttonIcons.emplace_back(saveSprite);
}

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
void ReleaseFunctions(const std::string result, size_t buttonNumber, sf::RenderWindow& mainWindow, Image& image, FileField& fileField, StatusBar& statusBar) {
    switch (static_cast<Buttons>(buttonNumber)) {
        case (Buttons::AddFile): {
            if (!result.empty()) {
                try {
                    std::cout << "0" << '\n';
                    image.LoadImage(result);
                    std::cout << "1" << '\n';
                    image.DrawImage(mainWindow);
                    std::cout << "2" << '\n';
                    fileField.AddFile(GetFileName(result));
                    std::cout << "3" << '\n';

                    statusBar.UpdateStatus("image loaded successfully");
                } catch (std::exception& e) {
                    statusBar.UpdateStatus("Image could not be found");
                }
            }

            break;
        }

        case (Buttons::DeleteFile): {
            if (!result.empty()) {
                size_t oldLength{fileField.GetFiles().size()};  // old count of files
                fileField.DeleteFile(result);
                size_t newLength{fileField.GetFiles().size()};  // new count of files

                image.ClearImage();

                if (oldLength == newLength) {
                    statusBar.UpdateStatus("File not found");
                } else {
                    statusBar.UpdateStatus("The file was deleted successfully");
                }
            }

            break;
        }

        case (Buttons::SaveFile): {
            if (!result.empty() && image.SaveImage(result)) {
                statusBar.UpdateStatus("The file was saved successfully");
            } else {
                statusBar.UpdateStatus("Unable to save the file");
            }

            break;
        }
    }
}
