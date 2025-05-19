#include "../Interface.hpp"

namespace Interface {
sf::Font LoadMainFont() {
    sf::Font mainFont;
    if (!mainFont.loadFromFile("../WindowFiles/Fonts/open-sans.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    return mainFont;
}

Zoom LoadZoomImages() {
    Zoom images;

    images.zoomOut = std::make_unique<Image>();
    images.zoomOut->LoadImage("../WindowFiles/Images/zoomOut-image.png");
    images.zoomOut->SetScale(kZoomImageScale, kZoomImageScale);

    images.zoomOut->SetPosition(kZoomOutPosX, kZoomOutPosY);

    images.zoomIn = std::make_unique<Image>();
    images.zoomIn->LoadImage("../WindowFiles/Images/zoomIn-image.png");
    images.zoomIn->SetScale(kZoomImageScale, kZoomImageScale);

    float zoomInPosX{kZoomOutPosX + images.zoomOut->GetSpriteBound().width};
    images.zoomIn->SetPosition(zoomInPosX, kZoomOutPosY);

    images.zoomBackground = std::make_unique<Image>();
    images.zoomBackground->LoadImage("../WindowFiles/Images/zoomBackground-image.png");
    images.zoomBackground->SetScale(kZoomBackgroundScaleX, kZoomBackgroundScaleY);

    images.zoomBackground->SetPosition(kZoomOutPosX - 2.5f, kZoomOutPosY);

    return images;
}

// Load Icons for buttons
std::tuple<IMAGES, ICONS> LoadButtonImages() {
    const NAMES paths{ADD_IMAGE, DELETE_IMAGE, SAVE_IMAGE, BRUSH_IMAGE, MOVE_IMAGE, RESET_IMAGE};
    IMAGES icons(paths.size());

    size_t i{};
    for (auto& icon : icons) {
        icon = std::make_shared<Image>();
        icon->LoadImage(paths[i++]);
    }

    ICONS iconShapes;  // Icon shapes and icons

    for (i = 0; i < icons.size(); ++i) {
        iconShapes.emplace_back(sf::Vector2f(kButtonWidth / 5.5f, kButtonWidth / 5.5f));
        iconShapes[i].setOrigin(0, iconShapes[i].getGlobalBounds().height / 2);
        iconShapes[i].setPosition(kSmallMenuWidth + kIconX + i * kButtonWidth, kButtonHeight / 2);

        iconShapes[i].setTexture(&icons[i]->GetTexture());
    }

    iconShapes[Buttons::AddFile].setPosition(kSmallMenuWidth + kIconX * 1.5f, kButtonHeight / 2);
    iconShapes[Buttons::Move].setPosition(5.55f * kButtonWidth, kButtonHeight / 2);
    iconShapes[Buttons::Reset].setPosition(iconShapes[Buttons::Move].getPosition().x + kButtonWidth, kButtonHeight / 2);

    return {icons, iconShapes};
}

MenuInterface CreateInterface() {
    return {CreateBackground(), CreateMenuImage(), CreateBrushSizeImage()};
}

BUTTONS CreateMenuButtons(const NAMES& names, const COLORS& colors, const sf::Font& font) {
    BUTTONS buttons{};

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
    menuImage.LoadImage("../WindowFiles/Images/smallMenu-image.png");
    menuImage.SetScale(kSmallMenuScale, kSmallMenuScale);
    menuImage.SetOrigin(menuImage.GetSpriteBound().width / 2, 0);
    menuImage.SetPosition(kSmallMenuIconPosition);

    return menuImage;
}

Image CreateBrushSizeImage() {
    Image brushSizeImage;
    brushSizeImage.LoadImage("../WindowFiles/Images/brush-size.png");
    brushSizeImage.SetScale(kBrushImageScale, kBrushImageScale);
    brushSizeImage.SetPosition(kBrushSizeImagePosition);

    return brushSizeImage;
}

BrushSizeDisplay CreateBrushSizeDisplay(const Image& brushSizeImage, sf::Font& font) {
    auto [x, y]{Interface::CalculateBrushSizePos(brushSizeImage)};  // Brush size field positions

    BrushSizeDisplay brushSizeField(kBrushInitialRadius, font);

    brushSizeField.SetPosition(x, y);
    brushSizeField.SetShapeSize({kBrushBoxWidth, kBrushBoxHeight});

    return brushSizeField;
}

BrushColorDisplay CreateBrushColorDisplay(const Image& brushSizeImage, Brush& brush) {
    auto [x, y]{Interface::CalculateBrushSizePos(brushSizeImage)};  // Brush size field positions

    BrushColorDisplay brushColorField(kBrushCurrentColorBoxSize);
    brushColorField.SetColor(brush.GetColor());

    auto [palettePosX, palettePosY]{Interface::CalculatePalettePos({x, kButtonHeight})};
    brushColorField.SetPalettePosition(palettePosX, palettePosY);

    // Brush current color shape position
    auto [brushColorShapePosX, brushColorShapePosY]{Interface::CalculateBrushColorShapePos({x, y})};
    brushColorField.SetPosition(brushColorShapePosX, brushColorShapePosY);

    return brushColorField;
}
}  // namespace Interface
