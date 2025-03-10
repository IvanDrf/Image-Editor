#include "Button.hpp"

#include "../MainWindow/MainWindow.hpp"

namespace {
const short KEnter{13};
const short kBackSpace{8};
}  // namespace

Button::Button(const float x, const float y, const std::string& name, sf::Font& font) {
    text_.setFont(font);
    text_.setCharacterSize(kCharacterSize);
    text_.setFillColor(sf::Color::White);
    text_.setString(name);

    float textWidth = text_.getGlobalBounds().width;
    float textHeight = text_.getGlobalBounds().height;

    text_.setOrigin(textWidth / 2, textHeight / 2);
    text_.setPosition(x + kButtonWidth / 2, y + kButtonHeight / 2 - textHeight / 2);

    shape_.setSize(sf::Vector2f(kButtonWidth, kButtonHeight));
    shape_.setFillColor(kFileButtonColor);
    shape_.setOutlineColor(sf::Color::Black);
    shape_.setOutlineThickness(3);
    shape_.setPosition(x, y);

}

void Button::CreateMenuButtons(std::vector<Button>& buttons, const std::vector<std::string>& names, sf::Font& font) {
    for (size_t i = 0; i < names.size(); ++i) {
        buttons.emplace_back(kSmallMenuWidth + kButtonWidth * i, 0, names[i], font);
    }
}

void Button::DrawButton(sf::RenderWindow& window) const {
    window.draw(shape_);
    window.draw(text_);
}

void Button::SetColor(const sf::Color& newColor) {
    shape_.setFillColor(newColor);
}

bool Button::PressButton(const sf::Vector2f& mousePosition) const {
    return shape_.getGlobalBounds().contains(mousePosition);
}

// Input Field after button was pressed

InputField::InputField(const float x, const float y) {
    if (!font_.loadFromFile("../WindowFiles/open-sans.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    text_.setFont(font_);
    text_.setCharacterSize(kCharacterSize);
    text_.setFillColor(sf::Color::Black);
    text_.setPosition(x, y);

    box_.setSize(sf::Vector2f(kBoxWidth, kBoxHeight));
    box_.setFillColor(sf::Color::White);
    box_.setOutlineColor(sf::Color::Black);
    box_.setOutlineThickness(1);
    box_.setPosition(x, y);
}

std::string InputField::Input(sf::Event& event, sf::RenderWindow& window) {
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

std::string InputField::CreateInputWindow(const std::string& heading, const std::string& inputText) {
    sf::RenderWindow window(sf::VideoMode(kSmallWindowWidth, kSmallWindowHeight), heading);
    InputField inputField(0, kSmallWindowHeight / 2);

    sf::Text text;
    text.setFont(inputField.font_);
    text.setCharacterSize(1.5 * kCharacterSize);
    text.setFillColor(sf::Color::White);
    text.setString(inputText);
    text.setPosition(0, 0);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return "";
            }

            std::string result{inputField.Input(event, window)};

            if (!result.empty()) {
                return result;
            }
        }

        window.clear();
        window.draw(text);
        inputField.Draw(window);
        window.display();
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

// Main buttons functions
std::string AddFile() {
    return InputField::CreateInputWindow("Add file", "Enter path to file");
}

std::string DeleteFile() {
    return InputField::CreateInputWindow("Delete file", "Enter file name");
}

std::string SaveFile() {
    return InputField::CreateInputWindow("Save file", "Enter file name");
}

std::string SelectFile() {
    return InputField::CreateInputWindow("Select file", "Enter file name");
}

std::string SelectBrush() {
}
