#pragma once

#include <SFML/Graphics.hpp>
#include <limits>

class Button {
 private:
    sf::RectangleShape shape_;
    sf::Text text_;

    static size_t activeButton;

 public:
    Button(const float x, const float y, const std::string& name, const sf::Color& color, const sf::Font& font);

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] sf::Color GetColor() const;

    void SetPosition(const float x, const float y);
    void SetColor(const sf::Color& newColor);

    void DrawButton(sf::RenderWindow& window) const;

    [[nodiscard]] bool AimButton(const sf::Vector2i& mousePosition) const;

    static size_t GetActiveButton();
    void AnimateButton(const sf::RenderWindow& window, size_t buttonIndex);

    static std::vector<Button> CreateMenuButtons(const std::vector<std::string>& names, const std::vector<sf::Color>& colors, const sf::Font& font);
};
