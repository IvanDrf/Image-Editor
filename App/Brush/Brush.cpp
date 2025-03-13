#include "Brush.hpp"

Brush::Brush(const float radius, const sf::Color& color) : radius_(radius), color_(color) {
    shape_.setRadius(radius);
    shape_.setFillColor(color);

    shape_.setOrigin(radius, radius);
}

void Brush::SetRadius(const float newRadius) {
    radius_ = newRadius;

    shape_.setRadius(newRadius);
    shape_.setOrigin(newRadius, newRadius);
}

float Brush::GetRadius() const {
    return radius_;
}

void Brush::SetColor(const sf::Color& newColor) {
    color_ = newColor;
    shape_.setFillColor(newColor);
}

sf::Color Brush::GetColor() const {
    return color_;
}

void Brush::Draw(sf::RenderTexture& texture, const sf::Vector2f& position) {
    shape_.setPosition(position);
    texture.draw(shape_);
}
