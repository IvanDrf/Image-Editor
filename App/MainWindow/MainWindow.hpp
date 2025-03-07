#pragma once

#include <SFML/Graphics.hpp>

using uint = unsigned int;
using ButtonFunction = std::string (*)();

enum class Buttons {
    AddFile = 0,
    DeleteFile = 1,
    SaveFile = 2,
    SelectFile = 3
};

namespace {  // Desktop Size
const uint kDesktopWidth{sf::VideoMode::getDesktopMode().width};
const uint kDesktopHeight{sf::VideoMode::getDesktopMode().height};
}  // namespace

namespace {  // Window Size
const float kMainWindowWidth{kDesktopWidth * 0.9f};
const float kMainWindowHeight{kDesktopHeight * 0.8f};
}  // namespace

namespace {  // File Field Size
const float kFileFieldWidth{kMainWindowWidth * 0.17f};
const float kFileFieldHeight{kMainWindowHeight * 0.955f};
}  // namespace

namespace {  // Status Bar Size
const float kStatusBarWidth{kMainWindowWidth - kFileFieldWidth};
const float kStatusBarHeight{kMainWindowHeight * 0.045f};

}  // namespace

namespace {  // Text Size
const short kCharacterSize{20};
}

namespace { // Zoom
const float kZoomIn{0.9f};
const float kZoomOut{1.1f};
}

namespace {  // Button
const float kButtonWidth{kMainWindowWidth / 10};
const float kButtonHeight{kMainWindowHeight - kFileFieldHeight};

const sf::Color kButtonColor{99, 139, 199};
}  // namespace

namespace {  // Small menu
const float kSmallMenuWidth{kFileFieldWidth / 4.8f};
const float kSmallMenuHeight{kButtonHeight};

const sf::Color kSmallMenuColor{40, 116, 254};
}  // namespace

namespace {  // Icon position
const float kIconX{5};
const float kIconY{kButtonHeight / 4};
}  // namespace

namespace {  // Small Window Size
const float kSmallWindowWidth{kMainWindowWidth * 0.25f};
const float kSmallWindowHeight{kMainWindowHeight * 0.2f};
}  // namespace

namespace {  // Box Size
const float kBoxWidth{kMainWindowWidth * 0.15f};
const float kBoxHeight{kMainWindowHeight * 0.03f};
}  // namespace

void LoadButtonImages(std::vector<sf::Sprite>& buttonIcons);
std::string GetFileName(const std::string& fileName);

// Needs for ReleaseFunctions
class Image;
class FileField;
class StatusBar;
void ReleaseFunctions(const std::string result, size_t buttonNumber, sf::RenderWindow& mainWindow, Image& image, FileField& FileField, StatusBar& StatusBar);

// Delete path
void DeletePath(std::vector<std::string>& pathToFile, const std::string& fileName);
std::string FindPath(std::vector<std::string>& pathToFile, const std::string& fileName);
