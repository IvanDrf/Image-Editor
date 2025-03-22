#pragma once

#include "../Image/Image.hpp"
#include "../InputField/InputField.hpp"
#include "SFML/Graphics.hpp"

namespace {
const int kBrushInitialRadius{10};  // Initial radius for brush
const int kBrushChangeRadius{5};    // Value of increasing or decreasing radius

const sf::Color kBrushInputSizeColor{152, 167, 212};
}  // namespace

class Brush {
 private:
    sf::CircleShape shape_;
    sf::Color color_;
    int radius_;

    sf::Texture brushCursorTexture;
    sf::Sprite brushCursorSprite;

 public:
    Brush(const int radius, const sf::Color& color);

    void SetRadius(const int newRadius);
    void UpdateCursorScale();

    [[nodiscard]] int GetRadius() const;
    [[nodiscard]] const sf::Sprite& GetBrushCursor() const;

    void SetColor(const sf::Color& newColor);
    [[nodiscard]] sf::Color GetColor() const;
    void Draw(sf::Image& image, const sf::Vector2f& position) const;

    void SetBrushCursor(const sf::Vector2i& mousePosition);
};
