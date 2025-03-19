#pragma once

#include <SFML/Graphics.hpp>
#include <stack>

class Image {
 private:
    sf::Image image_;

    sf::Texture texture_;
    sf::Sprite sprite_;

    bool hasImage_;

 public:
    Image();

    void DrawImage(sf::RenderWindow& window) const;
    void ClearImage(std::stack<sf::Image>& previousStatus);
    void UpdateTexture();

    void SaveState(std::stack<sf::Image>& previousStatus);  // Adding the modified image to the history
    void BackState(std::stack<sf::Image>& previousStatus);  // Deleting the modified image to the history

    void SetScale(const float scaleX, const float scaleY);
    void SetPosition(const float positionX, const float positionY);
    void SetPosition(const sf::Vector2f& position);
    void SetOrigin(const float centerX, const float centerY);
    void SetTexture(const sf::Texture& texture);

    [[nodiscard]] sf::FloatRect GetSpriteBound() const;
    const sf::Sprite& GetSprite() const;
    const sf::Texture& GetTexture() const;

    [[nodiscard]] sf::Vector2f GetImagePosition(const sf::Vector2f& mousePosition) const;
    [[nodiscard]] sf::Image& GetImage();

    void LoadImage(const std::string& filePath);
    void SetMainImageScale();

    [[nodiscard]] bool SaveImage(const std::string& fileName);
    [[nodiscard]] bool HasImage() const;

    operator sf::Image() const;
};
