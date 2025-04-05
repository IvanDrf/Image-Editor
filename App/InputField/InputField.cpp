#include "InputField.hpp"

#include "../MainWindow/MainWindow.hpp"

InputField::InputField() {
}

InputField::InputField(const float x, const float y, const sf::Vector2f& size) {
    if (!font_.loadFromFile("../WindowFiles/open-sans.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    text_.setFont(font_);
    text_.setCharacterSize(kCharacterSize);
    text_.setFillColor(sf::Color::Black);
    text_.setPosition(x, y);

    box_.setSize(size);
    box_.setFillColor(sf::Color::White);
    box_.setOutlineColor(sf::Color::Black);
    box_.setOutlineThickness(1);
    box_.setPosition(x, y);
}

void InputField::SetTextPosition(const float x, const float y) {
    text_.setPosition(x, y);
}

void InputField::SetFont(const sf::Font& font) {
    font_ = font;
    text_.setFont(font);
}

void InputField::SetText(const std::string& text) {
    text_.setString(text);
}

void InputField::SetTextColor(const sf::Color& color) {
    text_.setFillColor(color);
}

void InputField::SetTextSize(const int textSize) {
    text_.setCharacterSize(textSize);
}

void InputField::SetInputText(const std::string& inputString) {
    inputString_ = inputString;
}

void InputField::SetBoxPosition(const float x, const float y) {
    box_.setPosition(x, y);
}

void InputField::SetBoxSize(const sf::Vector2f& size) {
    box_.setSize(size);
}

void InputField::SetBoxColor(const sf::Color& color) {
    box_.setFillColor(color);
}

[[nodiscard]] sf::Vector2f InputField::GetPosition() {
    return box_.getPosition();
}

bool InputField::Active(const sf::Vector2f& mousePosition) const {
    return box_.getGlobalBounds().contains(mousePosition);
}

std::string InputField::Input(sf::Event& event, sf::RenderWindow& window, const Button& actionButton) {
    if (event.type == sf::Event::TextEntered) {
        char symbol{static_cast<char>(event.text.unicode)};
        if (symbol == KEnter) {  // Enter
            window.close();
            return inputString_;
        }

        if (symbol == kBackSpace && !inputString_.empty()) {  // Backspace
            inputString_.pop_back();
            text_.setString(inputString_);
        } else if (symbol != kBackSpace) {
            inputString_ += symbol;
            text_.setString(inputString_);
        }
    }

    return "";
}

void InputField::Clear() {
    inputString_ = "";
    text_.setString(inputString_);
}

void InputField::Draw(sf::RenderWindow& window) const {
    window.draw(box_);
    window.draw(text_);
}
