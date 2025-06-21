#pragma once

#include <SFML/Graphics.hpp>
#include <stack>

// clang-format off
#define NONE (std::numeric_limits<std::size_t>::max())

#define NAMES std::vector<std::string>
#define COLORS std::vector<sf::Color>

#define ADD_IMAGE    "../WindowFiles/Images/add-image.png"
#define DELETE_IMAGE "../WindowFiles/Images/delete-image.png"
#define SAVE_IMAGE   "../WindowFiles/Images/save-image.png"
#define BRUSH_IMAGE  "../WindowFiles/Images/brush-image.png"
#define MOVE_IMAGE   "../WindowFiles/Images/move-image.png"
#define RESET_IMAGE  "../WindowFiles/Images/reset-image.png"

#define IMAGES std::vector<std::shared_ptr<Image>>
#define ICONS std::vector<sf::RectangleShape>

#define BUTTONS std::vector<Button>

#define ADD_FILE "Add file"
#define DELETE_FILE "Delete file"
#define SAVE_FILE "Save file"
#define BRUSH "Brush"
#define MOVE "Move"
#define RESET "Reset"

#define BUTTONS_NAMES {ADD_FILE, DELETE_FILE, SAVE_FILE, BRUSH, MOVE, RESET}
#define BUTTONS_COLORS {kFileButtonColor, kFileButtonColor, kFileButtonColor, kToolsColor, kToolsColor, kToolsColor}
#define BUTTONS_FUNCTIONS {Interface::AddFile, Interface::DeleteFile, Interface::SaveFile, Interface::SelectBrush, Interface::MoveImage, Interface::Reset}

#define KEY_PRESSED sf::Keyboard::isKeyPressed
#define KEY event.key.code
#define FPS (30)

#define NO_ARGS [[maybe_unused]] Paths &pathsToFile, [[maybe_unused]] size_t activeFile
//clang format on

using uint = unsigned int;

using Paths = std::vector<std::string>;
using StackImage = std::stack<sf::Image>;

using ButtonFunction = std::string (*)(Paths&, size_t);

enum Buttons {
    AddFile = 0,
    DeleteFile = 1,
    SaveFile = 2,
    SelectBrush = 3,
    Move = 4,
    Reset = 5
};

// Constants

namespace {  // Desktop Size
const uint kDesktopWidth{sf::VideoMode::getDesktopMode().size.x};
const uint kDesktopHeight{sf::VideoMode::getDesktopMode().size.y};
}  // namespace

namespace {  // Interface Size
const uint kMainWindowWidth{static_cast<uint>(kDesktopWidth * 0.9f)};
const uint kMainWindowHeight{static_cast<uint>(kDesktopHeight * 0.8f)};
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
const float kButtonWidth{static_cast<float>(kMainWindowWidth / 10)};
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

namespace {  // Small Interface Size
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

namespace {  // Zoom image
const float kZoomOutPosX{kFileFieldWidth + kButtonWidth / 10};
const float kZoomOutPosY{1.25f * kButtonHeight};

constexpr float kZoomImageScale{0.35f};

constexpr float kDefaultZoom{1.0f};

constexpr float kMinZoom{1.f};
constexpr float kMaxZoom{5.f};

constexpr float kZoomStep{0.1f};

constexpr float kZoomBackgroundScaleX{0.4f};
constexpr float kZoomBackgroundScaleY{0.35f};
}  // namespace
