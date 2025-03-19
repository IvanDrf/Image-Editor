#pragma once

#include "../Image/Image.hpp"
#include "../InputField/InputField.hpp"
#include "SFML/Graphics.hpp"

namespace {
const float kBrushInitialRadius{10.0f};  // Initial radius for brush
const float kBrushChangeRadius{7.5f};
}  // namespace

class Brush : public Image, public InputField {
 private:
    sf::CircleShape shape_;
    sf::Color color_;
    float radius_;

    sf::Texture brushCursorTexture;
    sf::Sprite brushCursorSprite;

 public:
    Brush(const float radius, const sf::Color& color);

    void SetRadius(const float newRadius);
    void UpdateCursorScale();

    [[nodiscard]] float GetRadius() const;
    [[nodiscard]] const sf::Sprite& GetBrushCursor() const;

    void SetColor(const sf::Color& newColor);
    [[nodiscard]] sf::Color GetColor() const;
    void Draw(sf::Image& image, const sf::Vector2f& position) const;

    void SetBrushCursor(const sf::Vector2i& mousePosition);
};
