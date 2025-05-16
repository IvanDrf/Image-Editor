#include "Interface.hpp"

#include <limits>

#include "../MainWindow.hpp"

namespace {
const short kMaxPathLength{std::numeric_limits<short>::max() - 1};
}

namespace Interface {
sf::Font LoadMainFont() {
    sf::Font mainFont;
    if (!mainFont.loadFromFile("../WindowFiles/open-sans.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    return mainFont;
}

Zoom LoadZoomImages() {
    Zoom images;

    images.zoomOut = std::make_unique<Image>();
    images.zoomOut->LoadImage("../WindowFiles/zoomOut-image.png");
    images.zoomOut->SetScale(kZoomImageScale, kZoomImageScale);

    images.zoomOut->SetPosition(kZoomOutPosX, kZoomOutPosY);

    images.zoomIn = std::make_unique<Image>();
    images.zoomIn->LoadImage("../WindowFiles/zoomIn-image.png");
    images.zoomIn->SetScale(kZoomImageScale, kZoomImageScale);

    float zoomInPosX{kZoomOutPosX + images.zoomOut->GetSpriteBound().width};
    images.zoomIn->SetPosition(zoomInPosX, kZoomOutPosY);

    images.zoomBackground = std::make_unique<Image>();
    images.zoomBackground->LoadImage("../WindowFiles/zoomBackground-image.png");
    images.zoomBackground->SetScale(kZoomBackgroundScaleX, kZoomBackgroundScaleY);

    images.zoomBackground->SetPosition(kZoomOutPosX - 2.5f, kZoomOutPosY);

    return images;
}

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

    sf::Texture moveIcon;
    if (!moveIcon.loadFromFile("../WindowFiles/move-image.png")) {
        throw std::runtime_error("Move image could not be uploaded");
    }

    sf::Texture resetIcon;
    if (!resetIcon.loadFromFile("../WindowFiles/reset-image.png")) {
        throw std::runtime_error("Reset image could not be uploaded");
    }

    static const std::vector<sf::Texture> icons = {addIcon, deleteIcon, saveIcon, brushIcon, moveIcon, resetIcon};
    std::vector<sf::RectangleShape> iconShapes;  // Icon shapes and icons

    for (size_t i = 0; i < icons.size(); ++i) {
        iconShapes.emplace_back(sf::Vector2f(kButtonWidth / 5.5f, kButtonWidth / 5.5f));
        iconShapes[i].setOrigin(0, iconShapes[i].getGlobalBounds().height / 2);
        iconShapes[i].setPosition(kSmallMenuWidth + kIconX + i * kButtonWidth, kButtonHeight / 2);

        iconShapes[i].setTexture(&icons[i]);
    }

    iconShapes[Buttons::AddFile].setPosition(kSmallMenuWidth + kIconX * 1.5f, kButtonHeight / 2);
    iconShapes[Buttons::Move].setPosition(5.55f * kButtonWidth, kButtonHeight / 2);
    iconShapes[Buttons::Reset].setPosition(iconShapes[Buttons::Move].getPosition().x + kButtonWidth, kButtonHeight / 2);

    return iconShapes;
}

MenuInterface CreateInterface() {
    return {CreateBackground(), CreateMenuImage(), CreateBrushSizeImage()};
}

std::vector<Button> CreateMenuButtons(const std::vector<std::string>& names, const std::vector<sf::Color>& colors, const sf::Font& font) {
    std::vector<Button> buttons{};

    for (size_t i = 0; i < names.size(); ++i) {
        buttons.emplace_back(kSmallMenuWidth + kButtonWidth * i, 0, names[i], colors[i], font);
    }

    buttons[Buttons::Move].SetPosition(5.5f * kButtonWidth, 0);                                     // Move button position
    buttons[Buttons::Reset].SetPosition(buttons[Buttons::Move].GetPosition().x + kButtonWidth, 0);  // Reset button position

    return buttons;
}

sf::RectangleShape CreateBackground() {
    sf::RectangleShape background;
    background.setFillColor(kToolsColor);
    background.setSize(sf::Vector2f(kMainWindowWidth, kSmallMenuHeight));
    background.setPosition(0, 0);

    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(kDefaultOutlineThickness);

    return background;
}

Image CreateMenuImage() {
    Image menuImage;
    menuImage.LoadImage("../WindowFiles/smallMenu-image.png");
    menuImage.SetScale(kSmallMenuScale, kSmallMenuScale);
    menuImage.SetOrigin(menuImage.GetSpriteBound().width / 2, 0);
    menuImage.SetPosition(kSmallMenuIconPosition);

    return menuImage;
}

Image CreateBrushSizeImage() {
    Image brushSizeImage;
    brushSizeImage.LoadImage("../WindowFiles/brush-size.png");
    brushSizeImage.SetScale(kBrushImageScale, kBrushImageScale);
    brushSizeImage.SetPosition(kBrushSizeImagePosition);

    return brushSizeImage;
}

BrushSizeDisplay CreateBrushSizeDisplay(const float x, const float y, sf::Font& font) {
    BrushSizeDisplay brushSizeField(kBrushInitialRadius, font);
    bool brushSizeFieldPressed{false};

    brushSizeField.SetPosition(x, y);
    brushSizeField.SetShapeSize({kBrushBoxWidth, kBrushBoxHeight});

    return brushSizeField;
}

BrushColorDisplay CreateBrushColorDisplay(const float x, const float y, Brush& brush) {
    BrushColorDisplay brushColorField(kBrushCurrentColorBoxSize);
    brushColorField.SetColor(brush.GetColor());

    auto [palettePosX, palettePosY]{Interface::CalculatePalettePos({x, kButtonHeight})};
    brushColorField.SetPalettePosition(palettePosX, palettePosY);

    // Brush current color shape position
    auto [brushColorShapePosX, brushColorShapePosY]{Interface::CalculateBrushColorShapePos({x, y})};
    brushColorField.SetPosition(brushColorShapePosX, brushColorShapePosY);

    return brushColorField;
}

Position CalculateBrushSizePos(const Image& image) {
    return {kSmallMenuWidth + 4 * kButtonWidth + 0.9f * image.GetSpriteBound().width + kButtonWidth / 5, (kButtonHeight - kBrushBoxHeight) / 2};
}

Position CalculateBrushColorShapePos(const Position& position) {
    return {position.x + 1.35f * kBrushBoxWidth, position.y};
}

Position CalculatePalettePos(const Position& position) {
    return {position.x, position.y + kDefaultOutlineThickness};
}

std::string AddFileWindow() {
    std::string command = "zenity --file-selection --title=\"Select file\" 2>/dev/null";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "";
    }

    char buffer[kMaxPathLength];
    std::string result{};
    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }

    int status = pclose(pipe);
    if (status != 0) {
        return "";
    }

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

std::string SaveFileWindow(const std::string& fileName) {
    std::string command = "zenity --file-selection --save --title=\"Save File\" 2>/dev/null";

    if (!fileName.empty()) {
        command += " --filename=\"" + fileName + "\"";
    }

    command += " 2>/dev/null";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "";
    }

    char buffer[kMaxPathLength];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    int status = pclose(pipe);
    if (status != 0) {
        return "";
    }

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

std::string AddFile([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return AddFileWindow();
}

std::string DeleteFile(Paths& pathsToFile, size_t activeFile) {
    if (activeFile < pathsToFile.size()) {
        return pathsToFile[activeFile];
    }

    return "";
}

std::string SaveFile(Paths& pathsToFile, size_t activeFile) {
    if (activeFile < pathsToFile.size()) {
        return SaveFileWindow(pathsToFile[activeFile]);
    }

    return "";
}

std::string SelectBrush([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return "";
}

std::string MoveImage([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return "";
}

std::string Reset([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return "";
}
}  // namespace Interface
