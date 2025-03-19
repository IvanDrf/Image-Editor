#pragma once

#include <SFML/Graphics.hpp>

#include "../MainWindow/MainWindow.hpp"

class FileField {
 private:
    sf::RectangleShape shape_;
    sf::Font font_;

    std::vector<std::string> files_;

 public:
    FileField();

    void DrawField(sf::RenderWindow& window);
    void AddFile(const std::string& fileName);
    void DeleteFile(const std::string& fileName);

    [[nodiscard]] const std::vector<std::string>& GetFiles() const;
};
