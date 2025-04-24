#include "Interface.hpp"

#include "../MainWindow.hpp"

namespace Interface {
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

MenuInterface CreateInterface() {
    return {CreateBackground(), CreateMenuShape(), CreateMenuImage(), CreateBrushSizeImage()};
}

sf::RectangleShape CreateBackground() {
    sf::RectangleShape background;
    background.setFillColor(kToolsColor);
    background.setSize(sf::Vector2f(kMainWindowWidth, kSmallMenuHeight));
    background.setPosition(0, 0);

    return background;
}

sf::RectangleShape CreateMenuShape() {
    sf::RectangleShape menuShape;
    menuShape.setFillColor(kToolsColor);
    menuShape.setSize(sf::Vector2f(kSmallMenuWidth, kSmallMenuHeight));
    menuShape.setPosition(0, 0);

    menuShape.setOutlineColor(sf::Color::Black);
    menuShape.setOutlineThickness(kDefaultOutlineThickness);

    return menuShape;
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

[[nodiscard]] Zoom LoadZoomImages() {
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

Position CalculateBrushSizePos(const Image& image) {
    return {kSmallMenuWidth + 4 * kButtonWidth + 0.9f * image.GetSpriteBound().width + kButtonWidth / 5, (kButtonHeight - kBrushBoxHeight) / 2};
}

Position CalculateBrushColorShapePos(const Position& position) {
    return {position.x + 1.35f * kBrushBoxWidth, position.y};
}

Position CalculatePalettePos(const Position& position) {
    return {position.x, position.y + kDefaultOutlineThickness};
}

}  // namespace Interface
