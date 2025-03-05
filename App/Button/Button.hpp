#pragma once

#include <SFML/Graphics.hpp>

namespace {  // Button Color
const short kActiveRed{100};
const short kActiveGreen{100};
const short kActiveBlue{100};
}  // namespace

class Button {
 private:
    sf::RectangleShape shape_;
    sf::Color oldColor;

    sf::Clock clickTimer;
    bool isClicked;

    sf::Text text_;

 public:
    Button(const float x, const float y, const std::string& name, sf::Font& font);

    static void CreateMenuButtons(std::vector<Button>& buttons, const std::vector<std::string>& names, sf::Font& font);
    void DrawButton(sf::RenderWindow& window) const;

    bool PressButton(const sf::Vector2f& mousePosition) const;
    void ChangeColor();
    void ReturnColor();
};

std::string AddFile();
std::string DeleteFile();
std::string SaveFile();

// Input Field

namespace {
const short kMaxASCII{128};
}

class InputField {
 private:
    sf::RectangleShape box_;
    sf::Font font_;
    sf::Text text_;
    std::string inputString;

 public:
    InputField(const float x, const float y);

    std::string Input(sf::Event& event, sf::RenderWindow& window);
    static std::string CreateInputWindow(const std::string& heading, const std::string& inputText);

    void Clear();
    void Draw(sf::RenderWindow& window) const;
};
