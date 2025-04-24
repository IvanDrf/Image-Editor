#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "../../Image/Image.hpp"

// Functions
namespace Interface {
struct MenuInterface {
    sf::RectangleShape backGround;

    sf::RectangleShape menuShape;
    Image menuImage;

    Image brushSizeImage;
};

[[nodiscard]] std::vector<sf::RectangleShape> LoadButtonImages();

[[nodiscard]] MenuInterface CreateInterface();

[[nodiscard]] sf::RectangleShape CreateBackground();
[[nodiscard]] sf::RectangleShape CreateMenuShape();
[[nodiscard]] Image CreateMenuImage();
[[nodiscard]] Image CreateBrushSizeImage();

struct Zoom {
    std::unique_ptr<Image> zoomOut;
    std::unique_ptr<Image> zoomIn;

    std::unique_ptr<Image> zoomBackground;
};

[[nodiscard]] Zoom LoadZoomImages();

struct Position {
    float x = 0;
    float y = 0;
};

[[nodiscard]] Position CalculateBrushSizePos(const Image& image);
[[nodiscard]] Position CalculateBrushColorShapePos(const Position& position);
[[nodiscard]] Position CalculatePalettePos(const Position& position);

}  // namespace Interface
