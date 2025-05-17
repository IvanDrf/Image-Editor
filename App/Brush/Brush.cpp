#include "Brush.hpp"

#include "../MainWindow/MainWindow.hpp"

Brush::Brush() : radius_(kBrushInitialRadius), color_(sf::Color::White) {
    if (!brushCursorTexture.loadFromFile("../WindowFiles/brush-cursor.png")) {
        throw std::runtime_error("Brush cursor could not be uploaded");
    }
    brushCursorSprite.setTexture(brushCursorTexture);
    brushCursorSprite.setOrigin(brushCursorTexture.getSize().x / 2, brushCursorTexture.getSize().y / 2);

    UpdateCursorScale(1.f);
}

Brush::Brush(const int radius, const sf::Color& color) : Brush::Brush() {
    radius_ = radius;
    color_ = color;

    shape_.setRadius(radius);
    shape_.setFillColor(color);

    shape_.setOrigin(radius, radius);
}

void Brush::SetRadius(const int newRadius) {
    radius_ = newRadius;

    shape_.setRadius(newRadius);
    shape_.setOrigin(newRadius, newRadius);
}

void Brush::UpdateCursorScale(float currentScale) {
    const sf::Vector2u bounds{brushCursorTexture.getSize()};
    brushCursorSprite.setScale(2.8f * currentScale * radius_ / bounds.x, 2.8f * currentScale * radius_ / bounds.y);
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

void Brush::SetColor(const sf::Keyboard::Key& keyboardButton, BrushColorDisplay& brushCurrentColor) {
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

        case sf::Keyboard::Y: {
            color_ = sf::Color::Yellow;
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

    brushCurrentColor.SetColor(color_);
}

sf::Color Brush::GetColor() const {
    return color_;
}

void Brush::Draw(sf::Image& image, const sf::Vector2f& position) const {
    if (radius_ == 0) {
        return;
    }

    int radius{static_cast<int>(radius_)};
    sf::Vector2u imageSize{image.getSize()};

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

void Brush::DrawOnImage(AppData& appData, const sf::Event& event) {
    const sf::Vector2f imageBoundary{kFileFieldWidth + appData.image.GetSpriteBound().width, kButtonWidth + appData.image.GetSpriteBound().height};

    if (event.mouseMove.x >= kFileFieldWidth && event.mouseMove.x <= imageBoundary.x && event.mouseMove.y >= kButtonHeight && event.mouseMove.y <= imageBoundary.y) {
        appData.image.SaveState(appData.previousStatus);
        appData.brush.Draw(appData.image.GetImage(), appData.image.GetImagePosition({event.mouseMove.x, event.mouseMove.y}));

        appData.image.UpdateTexture();
    }
}

void Brush::InputBrushSize(AppData& appData, sf::Event& event, BrushSizeDisplay& brushSizeField) {
    appData.brush.SetRadius(brushSizeField.InputSize(event));
    brushSizeField.SetText(appData.brush.GetRadius());

    appData.brush.UpdateCursorScale(appData.currentZoom);
}

void Brush::IncreaseBrushSize(AppData& appData, BrushSizeDisplay& brushSizeField) {
    appData.brush.SetRadius(std::min(appData.brush.GetRadius() + kBrushChangeRadius, kBrushMaxRadius));
    brushSizeField.SetText(appData.brush.GetRadius());

    appData.brush.UpdateCursorScale(appData.currentZoom);
}

void Brush::DecreaseBrushSize(AppData& appData, BrushSizeDisplay& brushSizeField) {
    appData.brush.SetRadius(std::max(kBrushInitialRadius / kBrushInitialRadius, appData.brush.GetRadius() - kBrushChangeRadius));
    brushSizeField.SetText(appData.brush.GetRadius());

    appData.brush.UpdateCursorScale(appData.currentZoom);
}

void Brush::SetBrushCursor(const sf::Vector2i& mousePosition) {
    brushCursorSprite.setPosition(mousePosition.x, mousePosition.y);
}

// Brush size display

BrushSizeDisplay::BrushSizeDisplay(const int size, const sf::Font& font) {
    font_ = font;

    size_.setFont(font_);
    size_.setCharacterSize(kBrushSizeFieldCharacterSize);
    size_.setFillColor(sf::Color::White);
    size_.setString(std::to_string(size));

    shape_.setFillColor(kBrushInputSizeColor);
}

void BrushSizeDisplay::SetText(const int size) {
    size_.setString(std::to_string(size));
}

void BrushSizeDisplay::SetShapeSize(const sf::Vector2f& shapeSize) {
    shape_.setSize(shapeSize);
}

void BrushSizeDisplay::SetPosition(const float x, const float y) {
    shape_.setPosition(x, y);
    size_.setPosition(x, y);
}

bool BrushSizeDisplay::ShapeClicked(const sf::Vector2i& mousePosition) const {
    return shape_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

int BrushSizeDisplay::InputSize(sf::Event& event) {
    std::string newSize{size_.getString()};

    char symbol{static_cast<char>(event.text.unicode)};

    if (symbol == kBackSpace && !newSize.empty()) {  // Backspace
        newSize.pop_back();
    } else if (symbol != kBackSpace && symbol >= kMinInputNumber && symbol <= kMaxInputNumber) {
        newSize += symbol;
    }

    size_.setString(newSize);

    try {
        return (std::stoi(newSize) >= kBrushMaxRadius) ? (0) : (std::stoi(newSize));
    } catch (...) {
        return 0;
    }
}

void BrushSizeDisplay::Draw(sf::RenderWindow& window) const {
    window.draw(shape_);
    window.draw(size_);
}

// Brush color display

BrushColorDisplay::BrushColorDisplay(const sf::Vector2f& size) {
    shape_.setSize(size);

    shape_.setOutlineColor(sf::Color::White);
    shape_.setOutlineThickness(kDefaultOutlineThickness);

    palette_.LoadImage("../WindowFiles/palette-image.png");
    palette_.SetScale(kPaletteScale, kPaletteScale);

    auto bounds = palette_.GetSprite().getGlobalBounds();
    palette_.SetOrigin(bounds.width / 2, 0);
}

void BrushColorDisplay::SetColor(const sf::Color& color) {
    currentColor_ = color;
    shape_.setFillColor(color);
}

sf::Color BrushColorDisplay::GetPaletteColor(const sf::Vector2i& mousePosition) {
    sf::Vector2f localPos = palette_.GetSprite().getInverseTransform().transformPoint(static_cast<sf::Vector2f>(mousePosition));

    const auto& paletteImage = palette_.GetImage();

    unsigned pixelX = static_cast<unsigned>(std::clamp(localPos.x, 0.f, paletteImage.getSize().x - 1.0f));
    unsigned pixelY = static_cast<unsigned>(std::clamp(localPos.y, 0.f, paletteImage.getSize().y - 1.0f));

    return paletteImage.getPixel(pixelX, pixelY);
}

void BrushColorDisplay::SetPosition(const float x, const float y) {
    shape_.setPosition(x, y);
}

void BrushColorDisplay::SetPalettePosition(const float x, const float y) {
    palette_.SetPosition(x, y);
}

bool BrushColorDisplay::ShapeClicked(const sf::Vector2i& mousePosition) const {
    return shape_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

bool BrushColorDisplay::PaletteClicked(const sf::Vector2i& mousePosition) const {
    return palette_.GetSprite().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

void BrushColorDisplay::DrawPalette(sf::RenderWindow& window) const {
    palette_.DrawImage(window);
}

void BrushColorDisplay::Draw(sf::RenderWindow& window) const {
    window.draw(shape_);
}
