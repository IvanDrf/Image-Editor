#include "StatusBar.hpp"

StatusBar::StatusBar() {
    if (!font_.loadFromFile("../WindowFiles/Pragmatica.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    text_.setFont(font_);
    text_.setCharacterSize(kCharacterSize);
    text_.setString("");
    text_.setFillColor(sf::Color::White);

    text_.setPosition(kFileFieldWidth + 10, kMainWindowHeight - kStatusBarHeight + 5);

    shape_.setSize(sf::Vector2f(kStatusBarWidth, kStatusBarHeight));
    shape_.setFillColor(KSystemColor);
    shape_.setOutlineColor(sf::Color::Black);
    shape_.setOutlineThickness(3);

    shape_.setPosition(kFileFieldWidth + 3, kMainWindowHeight - kStatusBarHeight);
}

void StatusBar::DrawStatusBar(sf::RenderWindow& window) {
    window.draw(shape_);
    window.draw(text_);
}

void StatusBar::UpdateStatus(const std::string& newStatus, const sf::Color& textColor) {
    text_.setString(newStatus);

    if (text_.getFillColor() != textColor) {
        text_.setFillColor(textColor);
    }
}
