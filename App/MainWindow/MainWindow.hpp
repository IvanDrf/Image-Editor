#pragma once

#include <SFML/Graphics.hpp>
#include <stack>

using uint = unsigned int;
using ButtonFunction = std::string (*)(const std::vector<std::string>&, size_t);

enum Buttons {
    AddFile = 0,
    DeleteFile = 1,
    SaveFile = 2,
    SelectBrush = 3
};

// Constants

namespace {  // Desktop Size
const uint kDesktopWidth{sf::VideoMode::getDesktopMode().width};
const uint kDesktopHeight{sf::VideoMode::getDesktopMode().height};
}  // namespace

namespace {  // Window Size
const float kMainWindowWidth{kDesktopWidth * 0.9f};
const float kMainWindowHeight{kDesktopHeight * 0.8f};
}  // namespace

namespace {  // File Field
const float kFileFieldWidth{kMainWindowWidth * 0.17f};
const float kFileFieldHeight{kMainWindowHeight * 0.955f};

constexpr float kLineHeight{30};
const sf::Color kFileBackGroundColor{166, 168, 170};
const sf::Vector2f kFileBackGroundSize{kFileFieldWidth, 24};

}  // namespace

namespace {
const sf::Color KSystemColor{83, 83, 83};
}

namespace {  // Status Bar Size
const float kStatusBarWidth{kMainWindowWidth - kFileFieldWidth};
const float kStatusBarHeight{kMainWindowHeight * 0.045f};

}  // namespace

namespace {  // Text Size
constexpr short kCharacterSize{20};
}

namespace {  // Button
const float kButtonWidth{kMainWindowWidth / 10};
const float kButtonHeight{kMainWindowHeight - kFileFieldHeight};
constexpr short kDefaultOutlineThickness{2};

const sf::Color kFileButtonColor{99, 139, 199};
const sf::Color kToolsColor{72, 111, 180};
const sf::Color kActiveButtonColor{117, 167, 217};
}  // namespace

namespace {  // Small menu
const float kSmallMenuWidth{kFileFieldWidth / 4.8f};
const float kSmallMenuHeight{kButtonHeight};

constexpr float kSmallMenuScale{0.4f};                                    // Icon scale
const sf::Vector2f kSmallMenuIconPosition{kSmallMenuWidth / 3.6f, 1.5f};  // Icon position
}  // namespace

namespace {  // Icon position
constexpr float kIconX{5};
const float kIconY{kButtonHeight / 6.2f};
}  // namespace

namespace {  // Small Window Size
const float kSmallWindowWidth{kMainWindowWidth * 0.25f};
const float kSmallWindowHeight{kMainWindowHeight * 0.2f};
}  // namespace

namespace {  // Box Size
const float kBoxWidth{kMainWindowWidth * 0.15f};
const float kBoxHeight{kMainWindowHeight * 0.03f};

const float kBrushBoxWidth{kButtonWidth * 0.25f};
const float kBrushBoxHeight{kButtonHeight / 2.1f};
}  // namespace

namespace {  // Brush
constexpr float kBrushImageScale{0.45f};
constexpr float kBrushSizeFieldCharacterSize{kCharacterSize / 1.1f};

constexpr char kBackSpace{8};

const sf::Vector2f kBrushCurrentColorBoxSize{kBrushBoxHeight, kBrushBoxHeight};
const sf::Vector2f kBrushSizeImagePosition{kSmallMenuWidth + 4 * kButtonWidth + kButtonWidth / 10, 0};

constexpr float kPaletteScale{0.45f};
}  // namespace

using Paths = std::vector<std::string>;
using StackImage = std::stack<sf::Image>;

class Image;
class FileField;
class StatusBar;
class Brush;

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
