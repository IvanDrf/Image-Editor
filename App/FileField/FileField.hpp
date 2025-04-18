#pragma once

#include <SFML/Graphics.hpp>

#include "../Image/Image.hpp"
#include "../MainWindow/MainWindow.hpp"
#include "../StatusBar/StatusBar.hpp"

class FileField {
 public:
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

namespace ActiveFile {
struct ActiveContext {
    size_t& activeFile;
    size_t& previousFile;
    Paths& pathsToFile;
    Image& image;
    StackImage& previousStatus;
    StatusBar& statusBar;
};

void SelectActiveImage(ActiveContext& active);
void SelectUpperImage(ActiveContext& active);
void SelectLowerImage(ActiveContext& active);
}  // namespace ActiveFile
