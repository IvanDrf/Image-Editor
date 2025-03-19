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

void Brush::Draw(sf::Image& image, const sf::Vector2f& position) const {
    int radius = static_cast<int>(radius_);
    sf::Vector2u imageSize = image.getSize();

    const int radiusSquared{radius * radius};

    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            if (x * x + y * y <= radiusSquared) {
                int pixelX = static_cast<int>(position.x) + x;
                int pixelY = static_cast<int>(position.y) + y;

                if (pixelX >= 0 && pixelX < static_cast<int>(imageSize.x) && pixelY >= 0 && pixelY < static_cast<int>(imageSize.y)) {
                    image.setPixel(pixelX, pixelY, color_);
                }
            }
        }
    }
}
