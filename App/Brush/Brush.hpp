#pragma once

#include "SFML/Graphics.hpp"

namespace {
const int kBrushInitialRadius{10};  // Initial radius for brush
const int kBrushChangeRadius{5};

const sf::Color kBrushInputSizeColor{152, 167, 212};
}  // namespace

class Brush {
 private:
    sf::CircleShape shape_;
    sf::Color color_;
    float radius_;

    sf::Texture brushCursorTexture;
    sf::Sprite brushCursorSprite;

 public:
    Brush(const int radius, const sf::Color& color);

    void SetRadius(const int newRadius);
    void UpdateCursorScale();

    [[nodiscard]] int GetRadius() const;
    [[nodiscard]] const sf::Sprite& GetBrushCursor() const;

    void SetColor(const sf::Color& newColor);
    void SetColor(const sf::Keyboard::Key& keyboardButton, sf::RectangleShape& brushColorShape);

    [[nodiscard]] sf::Color GetColor() const;
    void Draw(sf::Image& image, const sf::Vector2f& position) const;

    void SetBrushCursor(const sf::Vector2i& mousePosition);
};

class BrushSizeDisplay {
 private:
    sf::Text size_; // Current brush size
    sf::Font font_;

    sf::RectangleShape shape_;

 public:
    BrushSizeDisplay(const int size, const sf::Font& font);

    void SetText(const int size); // Current brush Size

    void SetShapeSize(const sf::Vector2f& shapeSize);
    void SetPosition(const float x, const float y);

    void Draw(sf::RenderWindow& window) const;
};
