#include "Button.hpp"

#include "../MainWindow/MainWindow.hpp"

Button::Button(const float x, const float y, const std::string& name, const sf::Color& color, const sf::Font& font) {
    text_.setFont(font);
    text_.setCharacterSize(kCharacterSize);
    text_.setFillColor(sf::Color::White);
    text_.setString(name);

    float textWidth = text_.getGlobalBounds().width;
    float textHeight = text_.getGlobalBounds().height;

    text_.setOrigin(textWidth / 2, textHeight / 2);
    text_.setPosition(x + kButtonWidth / 2, y + kButtonHeight / 2 - textHeight / 2);

    shape_.setSize(sf::Vector2f(kButtonWidth, kButtonHeight));
    shape_.setFillColor(color);
    shape_.setOutlineColor(sf::Color::Black);
    shape_.setOutlineThickness(3);
    shape_.setPosition(x, y);
}

void Button::CreateMenuButtons(std::vector<Button>& buttons, const std::vector<std::string>& names, const std::vector<sf::Color>& colors, const sf::Font& font) {
    for (size_t i = 0; i < names.size(); ++i) {
        buttons.emplace_back(kSmallMenuWidth + kButtonWidth * i, 0, names[i], colors[i], font);
    }
}

void Button::DrawButton(sf::RenderWindow& window) const {
    window.draw(shape_);
    window.draw(text_);
}

void Button::SetColor(const sf::Color& newColor) {
    shape_.setFillColor(newColor);
}

bool Button::PressButton(const sf::Vector2f& mousePosition) const {
    return shape_.getGlobalBounds().contains(mousePosition);
}
