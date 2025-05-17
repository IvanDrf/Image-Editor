#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "../../Brush/Brush.hpp"
#include "../../Button/Button.hpp"
#include "../../Image/Image.hpp"

namespace Interface {
struct MenuInterface {
    sf::RectangleShape background;  // Buttons background

    Image menuImage;

    Image brushSizeImage;
};

struct Zoom {
    std::unique_ptr<Image> zoomOut;
    std::unique_ptr<Image> zoomIn;

    std::unique_ptr<Image> zoomBackground;
};

// Config
[[nodiscard]] sf::Font LoadMainFont();
[[nodiscard]] Zoom LoadZoomImages();
[[nodiscard]] std::vector<sf::RectangleShape> LoadButtonImages();

[[nodiscard]] MenuInterface CreateInterface();

[[nodiscard]] std::vector<Button> CreateMenuButtons(const std::vector<std::string>& names, const std::vector<sf::Color>& colors, const sf::Font& font);
[[nodiscard]] sf::RectangleShape CreateBackground();
[[nodiscard]] Image CreateMenuImage();

[[nodiscard]] Image CreateBrushSizeImage();
[[nodiscard]] BrushSizeDisplay CreateBrushSizeDisplay(const float x, const float y, sf::Font& font);
[[nodiscard]] BrushColorDisplay CreateBrushColorDisplay(const float x, const float y, Brush& brush);

// Calculation posiions
struct Position {
    float x = 0;
    float y = 0;
};

[[nodiscard]] Position CalculateBrushSizePos(const Image& image);
[[nodiscard]] Position CalculateBrushColorShapePos(const Position& position);
[[nodiscard]] Position CalculatePalettePos(const Position& position);

// Interface Windows

std::string AddFileWindow();  // Add file button

std::string SaveFileWindow(const std::string& fileName);  // Save file button
}  // namespace Interface
