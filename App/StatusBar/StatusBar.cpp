#include "StatusBar.hpp"

StatusBar::StatusBar() {
    if (!font_.loadFromFile("../WindowFiles/Pragmatica.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    text_.setFont(font_);
    text_.setCharacterSize(kCharacterSize);
    text_.setString("status: ");
    text_.setFillColor(sf::Color::White);

    text_.setPosition(kFileFieldWidth + 10, kMainWindowHeight - kStatusBarHeight + 5);

    shape_.setSize(sf::Vector2f(kStatusBarWidth, kStatusBarHeight));
    shape_.setFillColor(sf::Color(83, 83, 83));
    shape_.setOutlineColor(sf::Color::Black);
    shape_.setOutlineThickness(3);

    shape_.setPosition(kFileFieldWidth + 3, kMainWindowHeight - kStatusBarHeight);
}

void StatusBar::DrawStatusBar(sf::RenderWindow& window) {
    window.draw(shape_);
    window.draw(text_);
}

void StatusBar::UpdateStatus(const std::string& newStatus) {
    text_.setString("status: " + newStatus);
}
