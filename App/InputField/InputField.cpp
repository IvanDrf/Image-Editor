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

bool InputField::Active(const sf::Vector2f& mousePosition) const {
    return box_.getGlobalBounds().contains(mousePosition);
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
    InputField inputField(0, kSmallWindowHeight / 4.7f, sf::Vector2f(kBoxWidth, kBoxHeight));
    InputField saveField(0, kSmallWindowHeight / 1.5f, sf::Vector2f(kBoxWidth, kBoxHeight));

    sf::Text saveText;  // If button save file was pressed
    saveText.setFont(inputField.font_);
    saveText.setCharacterSize(1.5 * kCharacterSize);
    saveText.setFillColor(sf::Color::White);
    saveText.setString("Enter new file name");
    saveText.setPosition(0, kSmallWindowHeight / 2.2f);

    sf::Text text;
    text.setFont(inputField.font_);
    text.setCharacterSize(1.5 * kCharacterSize);
    text.setFillColor(sf::Color::White);
    text.setString(inputText);
    text.setPosition(0, 0);

    bool isInputFieldActive{true};
    std::string result;
    std::string newFileName;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return "";
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                // Check which box is active
                if (inputField.box_.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    isInputFieldActive = true;  // If the old name is being entered now
                } else if (saveField.box_.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    isInputFieldActive = false;  // If the new name is being entered now
                }
            }

            if (isInputFieldActive) {
                result = inputField.Input(event, window);
            } else if (heading == "Save file") {
                newFileName = saveField.Input(event, window);
            }

            // Save with new name or save with old
            if (!result.empty() || !newFileName.empty()) {
                window.close();
                return (newFileName.empty()) ? (result) : (result + '|' + newFileName);
            }
        }

        window.clear();
        window.draw(text);
        if (heading == "Save file") {
            saveField.Draw(window);
            window.draw(saveText);
        }

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
    return "";
}
