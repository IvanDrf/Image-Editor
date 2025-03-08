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

    void SetScale(const float scaleX, const float scaleY);
    void SetPosition(const float positionX, const float positionY);
    void SetPosition(const sf::Vector2f& position);
    void SetOrigin(const float centerX, const float centerY);

    sf::FloatRect GetSpriteBound() const;

    void Move(const sf::Vector2f& offset);

    void LoadImage(const std::string& filePath);
    void SetMainImageScale();

    bool SaveImage(const std::string& fileName);
    bool HasImage() const;
};
