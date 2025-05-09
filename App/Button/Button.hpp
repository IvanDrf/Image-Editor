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

    void SetPosition(const float x, const float y);

    void DrawButton(sf::RenderWindow& window) const;
    void SetColor(const sf::Color& newColor);
    sf::Color GetColor() const;

    [[nodiscard]] bool AimButton(const sf::Vector2i& mousePosition) const;

    static size_t GetActiveButton();
    void AnimateButton(const sf::RenderWindow& window, size_t buttonIndex);

    static void CreateMenuButtons(std::vector<Button>& buttons, const std::vector<std::string>& names, const std::vector<sf::Color>& colors, const sf::Font& font);
};
