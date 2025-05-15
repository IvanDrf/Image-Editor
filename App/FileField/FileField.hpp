#pragma once

#include <SFML/Graphics.hpp>

#include "../Back/Back.hpp"
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

// Active file
namespace ActiveFile {
void ChangeActiveFileNumber(size_t buttonNumber, size_t& activeFile, size_t files);  // Add file, delete file
void SelectActiveImage(AppData& active);                                             // Switch between files

void SelectUpperImage(AppData& active);
void SelectLowerImage(AppData& active);
}  // namespace ActiveFile
