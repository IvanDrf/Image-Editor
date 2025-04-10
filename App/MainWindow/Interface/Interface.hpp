#pragma once

#include <SFML/Graphics.hpp>

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

struct Position {
    float x;
    float y;
};

[[nodiscard]] Position CalculateBrushSizePos(const Image& image);
[[nodiscard]] Position CalculateBrushColorShapePos(const Position& position);

}  // namespace Interface
