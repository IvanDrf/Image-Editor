#pragma once

#include "SFML/Graphics.hpp"
#include "../InputField/InputField.hpp"
#include "../Image/Image.hpp"

namespace {
const float kBrushInitialRadius{10.0f}; // Initial radius for brush
}

class Brush : public Image, public InputField {
 private:
    sf::CircleShape shape_;
    sf::Color color_;
    float radius_;

 public:
    Brush(const float radius, const sf::Color& color);

    void SetRadius(const float newRadius);
    float GetRadius() const;

    void SetColor(const sf::Color& newColor);
    sf::Color GetColor() const;
    void Draw(sf::RenderTexture& texture, const sf::Vector2f& position);

};
