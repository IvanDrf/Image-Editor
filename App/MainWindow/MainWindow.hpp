#pragma once

#include <SFML/Graphics.hpp>

#include <stack>

using uint = unsigned int;
using ButtonFunction = std::string (*)();

enum class Buttons {
    AddFile = 0,
    DeleteFile = 1,
    SaveFile = 2,
    SelectFile = 3,
    SelectBrush = 4
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

namespace {  // Zoom
const float kZoomIn{0.9f};
const float kZoomOut{1.1f};
}  // namespace

namespace {  // Button
const float kButtonWidth{kMainWindowWidth / 10};
const float kButtonHeight{kMainWindowHeight - kFileFieldHeight};

const sf::Color kFileButtonColor{99, 139, 199};
const sf::Color kToolsColor{72, 111, 180};
}  // namespace

namespace {  // Small menu
const float kSmallMenuWidth{kFileFieldWidth / 4.8f};
const float kSmallMenuHeight{kButtonHeight};

const float kSmallMenuScale{0.4f};                                        // Icon scale
const sf::Vector2f kSmallMenuIconPosition{kSmallMenuWidth / 3.6f, 1.5f};  // Icon position
}  // namespace

namespace {  // Icon position
const float kIconX{5};
const float kIconY{kButtonHeight / 6.2f};
}  // namespace

namespace {  // Small Window Size
const float kSmallWindowWidth{kMainWindowWidth * 0.25f};
const float kSmallWindowHeight{kMainWindowHeight * 0.2f};
}  // namespace

namespace {  // Box Size
const float kBoxWidth{kMainWindowWidth * 0.15f};
const float kBoxHeight{kMainWindowHeight * 0.03f};

const float kBrushBoxWidth{kButtonWidth * 0.2f};
const float kBrushBoxHeight{kButtonHeight / 2.5f};
}  // namespace

namespace {
const float kBrushImageScale{0.45f};
}

std::vector<sf::RectangleShape> LoadButtonImages();
std::string GetFileName(const std::string& fileName);

// Needs for ReleaseFunctions
class Image;
class FileField;
class StatusBar;
class Brush;
void ReleaseFunctions(const std::string& result, size_t buttonNumber, sf::RenderWindow& mainWindow, Image& image, FileField& FileField, StatusBar& StatusBar, bool& brushPressed, std::stack<sf::Image>& previousStatus);

// Delete path
void DeletePath(std::vector<std::string>& pathToFile, const std::string& fileName);
[[nodiscard]] std::string FindPath(std::vector<std::string>& pathToFile, const std::string& fileName);
