#pragma once

#include <SFML/Graphics.hpp>

class Image {
 private:
    sf::Texture texture_;
    sf::Sprite sprite_;

    bool hasImage;

 public:
    void DrawImage(sf::RenderWindow& window) const;
    void ClearImage();

    void LoadImage(const std::string& filePath);
    bool SaveImage(const std::string& fileName);
    bool HasImage() const;
};
