#pragma once

#include "../Back/Back.hpp"
#include "../Image/Image.hpp"
#include "SFML/Graphics.hpp"

namespace {
constexpr int kBrushInitialRadius{10};  // Initial radius for brush
constexpr int kBrushMaxRadius{1'000};
constexpr int kBrushChangeRadius{5};

constexpr char kMinInputNumber{48};  // 0 in ASKII
constexpr char kMaxInputNumber{57};  // 9 in ASKII

const sf::Color kBrushInputSizeColor{152, 167, 212};
}  // namespace

class BrushSizeDisplay;
class BrushColorDisplay;

class Brush {
 private:
    sf::CircleShape shape_;
    sf::Color color_;
    float radius_;

    sf::Texture brushCursorTexture;
    sf::Sprite brushCursorSprite;

 public:
    Brush();
    Brush(const int radius, const sf::Color& color);

    void SetRadius(const int newRadius);
    void UpdateCursorScale(float currentScale);

    [[nodiscard]] int GetRadius() const;
    [[nodiscard]] const sf::Sprite& GetBrushCursor() const;

    void SetColor(const sf::Color& newColor);
    void SetColor(const sf::Keyboard::Key& keyboardButton, BrushColorDisplay& brushCurrentColor);

    [[nodiscard]] sf::Color GetColor() const;

    void Draw(sf::Image& image, const sf::Vector2f& position) const;

    static void DrawOnImage(AppData& appData, const sf::Event& event);

    static void InputBrushSize(AppData& appData, sf::Event& event, BrushSizeDisplay& brushSizeField);
    static void IncreaseBrushSize(AppData& appData, BrushSizeDisplay& brushSizeField);
    static void DecreaseBrushSize(AppData& appData, BrushSizeDisplay& brushSizeField);

    static void SetBrushColorKey(Brush& brush, BrushColorDisplay& brushColorField, const sf::Keyboard::Key& key);

    void SetBrushCursor(const sf::Vector2i& mousePosition);
};

class BrushSizeDisplay {
 private:
    sf::Text size_;  // Current brush size
    sf::Font font_;

    sf::RectangleShape shape_;

 public:
    BrushSizeDisplay(const int size, const sf::Font& font);

    void SetText(const int size);  // Current brush Size

    void SetShapeSize(const sf::Vector2f& shapeSize);
    void SetPosition(const float x, const float y);

    bool ShapeClicked(const sf::Vector2i& mousePosition) const;
    int InputSize(sf::Event& event);

    void Draw(sf::RenderWindow& window) const;
};

class BrushColorDisplay {
 private:
    sf::Color currentColor_;
    sf::RectangleShape shape_;

    Image palette_;

 public:
    BrushColorDisplay(const sf::Vector2f& size);

    void SetColor(const sf::Color& color);
    sf::Color GetPaletteColor(const sf::Vector2i& mousePosition);

    void SetPosition(const float x, const float y);
    void SetPalettePosition(const float x, const float y);

    bool ShapeClicked(const sf::Vector2i& mousePosition) const;
    bool PaletteClicked(const sf::Vector2i& mousePosition) const;

    void DrawPalette(sf::RenderWindow& window) const;
    void Draw(sf::RenderWindow& window) const;

    void SetPaletteColor(Brush& brush, const float x, const float y);
};
