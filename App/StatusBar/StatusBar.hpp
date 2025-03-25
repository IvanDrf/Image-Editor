#pragma once

#include "../MainWindow/MainWindow.hpp"
#include "SFML/Graphics.hpp"

class StatusBar {
 private:
    sf::RectangleShape shape_;

    sf::Text text_;
    sf::Font font_;

 public:
    StatusBar();

    void DrawStatusBar(sf::RenderWindow& window);
    void UpdateStatus(const std::string& newStatus, const sf::Color& textColor = sf::Color::White);
};
