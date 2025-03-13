#include "Image.hpp"

#include "../MainWindow/MainWindow.hpp"

void Image::DrawImage(sf::RenderWindow& window) const {
    if (hasImage) {
        window.draw(sprite_);
    }
}

void Image::ClearImage() {
    hasImage = false;

    texture_ = sf::Texture();
    sprite_ = sf::Sprite();
}

void Image::SetScale(const float scaleX, const float scaleY) {
    sprite_.setScale(scaleX, scaleY);
}

void Image::SetPosition(const float positionX, const float positionY) {
    sprite_.setPosition(positionX, positionY);
}

void Image::SetPosition(const sf::Vector2f& position) {
    sprite_.setPosition(position);
}

void Image::SetOrigin(const float centerX, const float centerY) {
    sprite_.setOrigin(centerX, centerY);
}

sf::FloatRect Image::GetSpriteBound() const {
    return sprite_.getGlobalBounds();
}

void Image::Move(const sf::Vector2f& offset) {
    sprite_.move(offset);
}

void Image::LoadImage(const std::string& filePath) {
    if (!texture_.loadFromFile(filePath)) {
        throw std::runtime_error("Image could not be found");
    }

    sprite_.setTexture(texture_);
    hasImage = true;
}

void Image::SetMainImageScale() {
    sprite_.setScale(1.f, 1.f);
    sprite_.setPosition(0.f, 0.f);

    sf::FloatRect imageBounds{sprite_.getGlobalBounds()};
    float scaleX{(kMainWindowWidth - kFileFieldWidth) / imageBounds.width};
    float scaleY{(kMainWindowHeight - kStatusBarHeight - kButtonHeight) / imageBounds.height};

    sprite_.setScale(scaleX, scaleY);
    sprite_.setPosition(kFileFieldWidth, kButtonHeight);
}

bool Image::SaveImage(const std::string& fileName) {
    size_t delimiter{fileName.find('|')}; // find delimeter oldName|newName
    sf::Image image{texture_.copyToImage()};

    if (delimiter != std::string::npos) {
        std::string newFileName{fileName.substr(delimiter + 1)};
        return hasImage && image.saveToFile(newFileName);
    }

    return hasImage && image.saveToFile(fileName);
}

bool Image::HasImage() const {
    return hasImage;
}
