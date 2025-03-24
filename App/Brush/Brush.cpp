#include "Brush.hpp"

Brush::Brush(const int radius, const sf::Color& color) : radius_(radius), color_(color) {
    if (!brushCursorTexture.loadFromFile("../WindowFiles/brush-cursor.png")) {
        throw std::runtime_error("Brush cursor could not be uploaded");
    }
    brushCursorSprite.setTexture(brushCursorTexture);
    brushCursorSprite.setOrigin(brushCursorTexture.getSize().x / 2, brushCursorTexture.getSize().y / 2);

    UpdateCursorScale();

    shape_.setRadius(radius);
    shape_.setFillColor(color);

    shape_.setOrigin(radius, radius);
}

void Brush::SetRadius(const int newRadius) {
    radius_ = newRadius;

    shape_.setRadius(newRadius);
    shape_.setOrigin(newRadius, newRadius);
}

void Brush::UpdateCursorScale() {
    const sf::Vector2u bounds{brushCursorTexture.getSize()};
    brushCursorSprite.setScale(2.5f * radius_ / bounds.x, 2.5f * radius_ / bounds.y);
}

int Brush::GetRadius() const {
    return radius_;
}

const sf::Sprite& Brush::GetBrushCursor() const {
    return brushCursorSprite;
}

void Brush::SetColor(const sf::Color& newColor) {
    color_ = newColor;
    shape_.setFillColor(newColor);
}

void Brush::SetColor(const sf::Keyboard::Key& keyboardButton, sf::RectangleShape& brushColorShape) {
    switch (keyboardButton) {
        case sf::Keyboard::R: {
            color_ = sf::Color::Red;
            break;
        }

        case sf::Keyboard::G: {
            color_ = sf::Color::Green;
            break;
        }

        case sf::Keyboard::B: {
            color_ = sf::Color::Blue;
            break;
        }

        case sf::Keyboard::W: {
            color_ = sf::Color::White;
            break;
        }

        default: {
            return;
        }
    }

    brushColorShape.setFillColor(color_);
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

void Brush::SetBrushCursor(const sf::Vector2i& mousePosition) {
    brushCursorSprite.setPosition(mousePosition.x, mousePosition.y);
}
