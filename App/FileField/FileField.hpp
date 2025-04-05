#pragma once

#include <SFML/Graphics.hpp>

#include "../MainWindow/MainWindow.hpp"

class FileField {
 private:
    sf::RectangleShape activeFileBackGround_;
    sf::RectangleShape shape_;
    sf::Font font_;

    std::vector<std::string> files_;
    std::vector<sf::Text> displayedFiles_;

 public:
    FileField();

    void DrawField(sf::RenderWindow& window, size_t activeFile);
    void AddFile(const std::string& fileName);
    void DeleteFile(const std::string& fileName);

    [[nodiscard]] size_t GetActiveFile(const sf::Vector2i& mousePosition, size_t activeFile) const;
    [[nodiscard]] const std::vector<std::string>& GetFiles() const;
};
