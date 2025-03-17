#pragma once

#include <SFML/Graphics.hpp>
#include <stack>

class Image {
 private:
    sf::Image image_;
    std::stack<sf::Image> previousStatus;  // Story for (Ctrl+Z)

    sf::Texture texture_;
    sf::Sprite sprite_;

    bool hasImage_;

 public:
    Image();

    void DrawImage(sf::RenderWindow& window) const;
    void ClearImage();
    void UpdateTexture();

    void SaveState();  // Adding the modified image to the history
    void BackState();  // Deleting the modified image to the history

    void SetScale(const float scaleX, const float scaleY);
    void SetPosition(const float positionX, const float positionY);
    void SetPosition(const sf::Vector2f& position);
    void SetOrigin(const float centerX, const float centerY);
    void SetTexture(const sf::Texture& texture);

    sf::FloatRect GetSpriteBound() const;
    const sf::Sprite& GetSprite() const;
    const sf::Texture& GetTexture() const;

    sf::Vector2f GetImagePosition(const sf::Vector2f& mousePosition) const;
    sf::Image& GetImage();

    void LoadImage(const std::string& filePath);
    void SetMainImageScale();

    bool SaveImage(const std::string& fileName);
    bool HasImage() const;

    operator sf::Image() const;
};
