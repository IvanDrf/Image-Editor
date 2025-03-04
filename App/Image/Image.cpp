#include "Image.hpp"

#include "../MainWindow/MainWindow.hpp"

void Image::DrawImage(sf::RenderWindow& window) const {
    if (hasImage) {
        window.draw(sprite_);
    }
}

void Image::ClearImage() {
    hasImage = false;
}

void Image::LoadImage(const std::string& filePath) {
    if (!texture_.loadFromFile(filePath)) {
        throw std::runtime_error("Image could not be found");
    }

    sprite_.setTexture(texture_);
    hasImage = true;

    sf::FloatRect imageBounds{sprite_.getGlobalBounds()};
    float scaleX{(kMainWindowWidth - kFileFieldWidth) / imageBounds.width};
    float scaleY{(kMainWindowHeight - kStatusBarHeight - kButtonHeight) / imageBounds.height};

    float scale{std::min(scaleX, scaleY)};

    sprite_.setScale(scale, scale);
    sprite_.setPosition(kFileFieldWidth, kButtonHeight);
}

bool Image::SaveImage(const std::string& fileName) {
    sf::Image image{texture_.copyToImage()};

    return image.saveToFile(fileName);
}

bool Image::HasImage() const {
    return hasImage;
}
