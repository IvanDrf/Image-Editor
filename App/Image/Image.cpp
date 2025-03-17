#include "Image.hpp"

#include "../MainWindow/MainWindow.hpp"

Image::Image() : hasImage_(false), previousStatus() {
}

void Image::DrawImage(sf::RenderWindow& window) const {
    if (hasImage_) {
        window.draw(sprite_);
    }
}

void Image::ClearImage() {
    hasImage_ = false;
    texture_ = sf::Texture();
    sprite_ = sf::Sprite();
    image_ = sf::Image();

    while (!previousStatus.empty()) {
        previousStatus.pop();
    }
}

void Image::UpdateTexture() {
    texture_.loadFromImage(image_);
    sprite_.setTexture(texture_);
}

void Image::SaveState() {
    previousStatus.push(image_);
}

void Image::BackState() {
    if (previousStatus.empty()) {
        return;
    }

    image_ = previousStatus.top();
    previousStatus.pop();

    UpdateTexture();
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

void Image::SetTexture(const sf::Texture& texture) {
    texture_ = texture;
    sprite_.setTexture(texture_);
    hasImage_ = true;
}

sf::FloatRect Image::GetSpriteBound() const {
    return sprite_.getGlobalBounds();
}

const sf::Sprite& Image::GetSprite() const {
    return sprite_;
}

const sf::Texture& Image::GetTexture() const {
    return texture_;
}

sf::Vector2f Image::GetImagePosition(const sf::Vector2f& mousePosition) const {
    sf::FloatRect bounds = sprite_.getGlobalBounds();

    float imageX = (mousePosition.x - bounds.left) / sprite_.getScale().x;
    float imageY = (mousePosition.y - bounds.top) / sprite_.getScale().y;

    return sf::Vector2f(imageX, imageY);
}

sf::Image& Image::GetImage() {
    if (!hasImage_) {
        throw std::runtime_error("No image loaded");
    }
    return image_;
}

void Image::LoadImage(const std::string& filePath) {
    if (!image_.loadFromFile(filePath)) {
        throw std::runtime_error("Image could not be found");
    }
    texture_.loadFromImage(image_);
    sprite_.setTexture(texture_);
    hasImage_ = true;
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
    size_t delimiter{fileName.find('|')};  // find delimiter oldName|newName

    if (delimiter != std::string::npos) {
        std::string newFileName{fileName.substr(delimiter + 1)};
        return hasImage_ && image_.saveToFile(newFileName);
    }

    return hasImage_ && image_.saveToFile(fileName);
}

bool Image::HasImage() const {
    return hasImage_;
}

Image::operator sf::Image() const {
    return image_;
}
