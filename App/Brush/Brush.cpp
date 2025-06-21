#include "Brush.hpp"

#include <SFML/Config.hpp>
#include <algorithm>

#include "../MainWindow/MainWindow.hpp"

Brush::Brush() : radius_(kBrushInitialRadius), color_(sf::Color::White), brushCursorSprite_(brushCursorTexture_) {
    if (!brushCursorTexture_.loadFromFile("../WindowFiles/Images/brush-cursor.png")) {
        throw std::runtime_error("Brush cursor could not be uploaded");
    }
    brushCursorSprite_.setTexture(brushCursorTexture_);
    brushCursorSprite_.setOrigin(sf::Vector2f{brushCursorTexture_.getSize().x / 2, brushCursorTexture_.getSize().y / 2});

    UpdateCursorScale(1.f);
}

Brush::Brush(const int radius, const sf::Color& color) : Brush::Brush() {
    radius_ = radius;
    color_ = color;

    shape_.setRadius(radius);
    shape_.setFillColor(color);

    shape_.setOrigin({radius, radius});
}

void Brush::SetRadius(const int newRadius) {
    radius_ = newRadius;

    shape_.setRadius(newRadius);
    shape_.setOrigin({newRadius, newRadius});
}

void Brush::UpdateCursorScale(float currentScale) {
    const auto bounds{brushCursorTexture_.getSize()};
    brushCursorSprite_.setScale({2.8f * currentScale * radius_ / bounds.x, 2.8f * currentScale * radius_ / bounds.y});
}

int Brush::GetRadius() const {
    return radius_;
}

const sf::Sprite& Brush::GetBrushCursor() const {
    return brushCursorSprite_;
}

void Brush::SetColor(const sf::Color& newColor) {
    color_ = newColor;
    shape_.setFillColor(newColor);
}

void Brush::SetColor(const sf::Keyboard::Key& keyboardButton, BrushColorDisplay& brushCurrentColor) {
    switch (keyboardButton) {
        case sf::Keyboard::Key::R: {
            color_ = sf::Color::Red;
            break;
        }

        case sf::Keyboard::Key::G: {
            color_ = sf::Color::Green;
            break;
        }

        case sf::Keyboard::Key::B: {
            color_ = sf::Color::Blue;
            break;
        }

        case sf::Keyboard::Key::Y: {
            color_ = sf::Color::Yellow;
            break;
        }

        case sf::Keyboard::Key::W: {
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
    if (radius_ <= 0) {
        return;
    }

    const auto [width, height] = static_cast<sf::Vector2i>(image.getSize());
    const auto [centerX, centerY] = static_cast<sf::Vector2i>(position);

    const int radiusSquared = radius_ * radius_;

    for (int y = std::max(0, centerY - radius_); y <= std::min(height - 1, centerY + radius_); y++) {
        for (int x = std::max(0, centerX - radius_); x <= std::min(width - 1, centerX + radius_); x++) {
            const int dx = x - centerX;
            const int dy = y - centerY;
            if (dx * dx + dy * dy <= radiusSquared) {
                image.setPixel({x, y}, color_);
            }
        }
    }
}

void Brush::DrawOnImage(AppData& appData, const std::optional<sf::Event>& event) {
    auto& image{appData.image};
    auto& brush{appData.brush};
    auto& previousStatus{appData.previousStatus};

    const sf::Vector2f imageBoundary{kFileFieldWidth + image.GetSpriteBound().size.x, kButtonWidth + image.GetSpriteBound().size.y};

    image.SaveState(previousStatus);

    const auto mousePos{event->getIf<sf::Event::MouseMoved>()->position};

    if (mousePos.x >= kFileFieldWidth && mousePos.x <= imageBoundary.x && mousePos.y >= kButtonHeight && mousePos.y <= imageBoundary.y) {
        brush.Draw(image.GetImage(), image.GetImagePosition({mousePos.x, mousePos.y}));

        image.UpdateTexture();
    }
}

void Brush::InputBrushSize(AppData& appData, std::optional<sf::Event>& event, BrushSizeDisplay& brushSizeField) {
    auto& brush{appData.brush};
    auto& currentZoom{appData.currentZoom};

    brush.SetRadius(brushSizeField.InputSize(event));
    brushSizeField.SetText(brush.GetRadius());

    brush.UpdateCursorScale(currentZoom);
}

void Brush::IncreaseBrushSize(AppData& appData, BrushSizeDisplay& brushSizeField) {
    auto& brush{appData.brush};
    auto& currentZoom{appData.currentZoom};

    brush.SetRadius(std::min(brush.GetRadius() + kBrushChangeRadius, kBrushMaxRadius));
    brushSizeField.SetText(brush.GetRadius());

    brush.UpdateCursorScale(currentZoom);
}

void Brush::DecreaseBrushSize(AppData& appData, BrushSizeDisplay& brushSizeField) {
    auto& brush{appData.brush};
    auto& currentZoom{appData.currentZoom};

    brush.SetRadius(std::max(kBrushInitialRadius / kBrushInitialRadius, brush.GetRadius() - kBrushChangeRadius));
    brushSizeField.SetText(brush.GetRadius());

    brush.UpdateCursorScale(currentZoom);
}

void Brush::SetBrushColorKey(Brush& brush, BrushColorDisplay& brushColorField, const sf::Keyboard::Key& key) {
    brush.SetColor(key, brushColorField);
    brushColorField.SetColor(brush.GetColor());
}

void Brush::SetBrushCursor(const sf::Vector2i& mousePosition) {
    brushCursorSprite_.setPosition({mousePosition.x, mousePosition.y});
}

// Brush size display

BrushSizeDisplay::BrushSizeDisplay(const int size, const sf::Font& font) : size_(font) {
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
    shape_.setPosition({x, y});
    size_.setPosition({x, y});
}

bool BrushSizeDisplay::ShapeClicked(const sf::Vector2i& mousePosition) const {
    return shape_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

int BrushSizeDisplay::InputSize(const std::optional<sf::Event>& event) {
    std::string newSize{size_.getString()};

    char symbol{static_cast<char>(event->getIf<sf::Event::TextEntered>()->unicode)};

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

    palette_.LoadImage("../WindowFiles/Images/palette-image.png");
    palette_.SetScale(kPaletteScale, kPaletteScale);

    auto bounds = palette_.GetSprite().getGlobalBounds();
    palette_.SetOrigin(bounds.size.x / 2, 0);
}

void BrushColorDisplay::SetColor(const sf::Color& color) {
    currentColor_ = color;
    shape_.setFillColor(color);
}

sf::Color BrushColorDisplay::GetPaletteColor(const sf::Vector2i& mousePosition) {
    auto localPos = palette_.GetSprite().getInverseTransform().transformPoint(static_cast<sf::Vector2f>(mousePosition));

    const auto& paletteImage = palette_.GetImage();

    uint pixelX = static_cast<unsigned>(std::clamp(localPos.x, 0.f, paletteImage.getSize().x - 1.0f));
    uint pixelY = static_cast<unsigned>(std::clamp(localPos.y, 0.f, paletteImage.getSize().y - 1.0f));

    return paletteImage.getPixel({pixelX, pixelY});
}

void BrushColorDisplay::SetPosition(const float x, const float y) {
    shape_.setPosition({x, y});
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

void BrushColorDisplay::SetPaletteColor(Brush& brush, const int x, const int y) {
    if (PaletteClicked({x, y})) {
        brush.SetColor(GetPaletteColor({x, y}));

        SetColor(brush.GetColor());
    }
}
