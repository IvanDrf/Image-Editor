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
    static void CreateMenuButtons(std::vector<Button>& buttons, const std::vector<std::string>& names, const std::vector<sf::Color>& colors, const sf::Font& font);

    void DrawButton(sf::RenderWindow& window) const;
    void SetColor(const sf::Color& newColor);

    [[nodiscard]] bool AimButton(const sf::Vector2f& mousePosition) const;

    static size_t GetActiveButton();
    void AnimateButton(const sf::RenderWindow& window, size_t buttonIndex);
};
