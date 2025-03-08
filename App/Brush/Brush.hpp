#pragma once

#include "SFML/Graphics.hpp"

class Brush {
 private:
    sf::CircleShape shape_;
    sf::Color color_;
    float radius_;
 public:
    Brush(const float radius, const sf::Color& color);

    void SetRadius(const float newRadius);
    void SetColor(const sf::Color& newColor);

    void Draw(sf::RenderTexture& texture, const sf::Vector2f& position);
    float GetSize() const;


};
