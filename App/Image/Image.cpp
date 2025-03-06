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

void Image::LoadImage(const std::string& filePath) {
    if (!texture_.loadFromFile(filePath)) {
        throw std::runtime_error("Image could not be found");
    }

    sprite_.setTexture(texture_);
    hasImage = true;

    sprite_.setScale(1.f, 1.f);
    sprite_.setPosition(0.f, 0.f);

    sf::FloatRect imageBounds{sprite_.getGlobalBounds()};
    float scaleX{(kMainWindowWidth - kFileFieldWidth) / imageBounds.width};
    float scaleY{(kMainWindowHeight - kStatusBarHeight - kButtonHeight) / imageBounds.height};

    sprite_.setScale(scaleX, scaleY);
    sprite_.setPosition(kFileFieldWidth, kButtonHeight);
}

bool Image::SaveImage(const std::string& fileName) {
    sf::Image image{texture_.copyToImage()};

    return hasImage && image.saveToFile(fileName);
}

bool Image::HasImage() const {
    return hasImage;
}
