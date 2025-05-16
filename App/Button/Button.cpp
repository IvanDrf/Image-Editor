#include "Button.hpp"

#include "../MainWindow/MainWindow.hpp"

size_t Button::activeButton = std::numeric_limits<size_t>::max();

Button::Button(const float x, const float y, const std::string& name, const sf::Color& color, const sf::Font& font) {
    text_.setFont(font);
    text_.setCharacterSize(kCharacterSize);
    text_.setFillColor(sf::Color::White);
    text_.setString(name);

    float textWidth{text_.getGlobalBounds().width};
    float textHeight{text_.getGlobalBounds().height};

    text_.setOrigin(textWidth / 2, textHeight / 2);
    text_.setPosition(x + kButtonWidth / 2, y + kButtonHeight / 2 - textHeight / 2);

    shape_.setSize(sf::Vector2f(kButtonWidth, kButtonHeight));
    shape_.setFillColor(color);
    shape_.setOutlineColor(sf::Color::Black);
    shape_.setOutlineThickness(kDefaultOutlineThickness);
    shape_.setPosition(x, y);
}

sf::Vector2f Button::GetPosition() const {
    return shape_.getPosition();
}

sf::Color Button::GetColor() const {
    return shape_.getFillColor();
}

void Button::SetPosition(const float x, const float y) {
    text_.setPosition(x + kButtonWidth / 2, y + kButtonHeight / 2 - text_.getGlobalBounds().height / 2);
    shape_.setPosition(sf::Vector2f(x, y));
}

void Button::SetColor(const sf::Color& newColor) {
    shape_.setFillColor(newColor);
}

void Button::DrawButton(sf::RenderWindow& window) const {
    window.draw(shape_);
    window.draw(text_);
}

bool Button::AimButton(const sf::Vector2i& mousePosition) const {
    return shape_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

size_t Button::GetActiveButton() {
    return activeButton;
}

void Button::AnimateButton(const sf::RenderWindow& window, size_t buttonIndex) {
    sf::Vector2f mousePosition{sf::Mouse::getPosition(window)};

    if (shape_.getGlobalBounds().contains(mousePosition)) {
        sf::Vector2f newSize{1.1f * kButtonWidth, 1.1f * kButtonHeight};
        sf::Vector2f oldSize{shape_.getSize()};

        sf::Vector2f move{(oldSize.x - newSize.x) / 2.0f, (oldSize.y - newSize.y) / 2.0f};

        shape_.setSize(newSize);
        shape_.setPosition(shape_.getPosition() + move);

        activeButton = buttonIndex;

        return;
    }

    sf::Vector2f oldSize{shape_.getSize()};
    sf::Vector2f newSize{kButtonWidth, kButtonHeight};

    sf::Vector2f move((oldSize.x - newSize.x) / 2.0f, (oldSize.y - newSize.y) / 2.0f);

    shape_.setSize(newSize);
    shape_.setPosition(shape_.getPosition() + move);

    if (activeButton == buttonIndex) {
        activeButton = std::numeric_limits<size_t>::max();
    }
}
