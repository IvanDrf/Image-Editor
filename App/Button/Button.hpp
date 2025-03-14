#pragma once

#include <SFML/Graphics.hpp>

class Button {
 private:
    sf::RectangleShape shape_;
    sf::Text text_;

 public:
    Button(const float x, const float y, const std::string& name, const sf::Color& color, const sf::Font& font);
    static void CreateMenuButtons(std::vector<Button>& buttons, const std::vector<std::string>& names, const std::vector<sf::Color>& colors, const sf::Font& font);

    void DrawButton(sf::RenderWindow& window) const;
    void SetColor(const sf::Color& newColor);

    bool PressButton(const sf::Vector2f& mousePosition) const;
};
