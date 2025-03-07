#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

// Interface
#include "Button/Button.hpp"
#include "FileField/FileField.hpp"
#include "Image/Image.hpp"
#include "MainWindow/MainWindow.hpp"
#include "StatusBar/StatusBar.hpp"

// Menu Buttons, realization in Button.cpp
std::string AddFile();
std::string DeleteFile();
std::string SaveFile();
std::string SelectFile();

std::string SelectBrush();
//

auto main(int, char**) -> int {
    sf::RenderWindow mainWindow(sf::VideoMode(kMainWindowWidth, kMainWindowHeight), "Image Editor");

    sf::Font buttonFont;  // Font for Buttons
    if (!buttonFont.loadFromFile("../WindowFiles/open-sans.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    // Interface: current image, field with files names, status bar
    Image image;
    FileField fileField;
    StatusBar statusBar;

    // Small menu
    sf::RectangleShape menuShape;
    menuShape.setFillColor(kSmallMenuColor);
    menuShape.setSize(sf::Vector2f(kSmallMenuWidth, kSmallMenuHeight));
    menuShape.setPosition(0, 0);
    menuShape.setOutlineColor(sf::Color::Black);
    menuShape.setOutlineThickness(3);

    Image menuImage;
    menuImage.LoadImage("../WindowFiles/smallMenu-image.png");
    menuImage.SetScale(kSmallMenuWidth / menuImage.GetSpriteBound().width, kSmallMenuHeight / menuImage.GetSpriteBound().height);
    menuImage.SetPosition(kIconX * 1.5f, -kIconY / 2.5f);
    //

    // Menu Buttons
    const std::vector<std::string> buttonNames{"Add file", "Delete file", "Save file", " Select file", "Brush"};
    std::vector<sf::Sprite> buttonIcons;
    LoadButtonImages(buttonIcons);

    std::vector<Button> buttons;  // Vector with buttons

    Button::CreateMenuButtons(buttons, buttonNames, buttonFont);
    ButtonFunction buttonFuntions[]{AddFile, DeleteFile, SaveFile, SelectFile, SelectBrush};

    float zoom{1.0f};  // Zoom for image

    // Main Loop
    while (mainWindow.isOpen()) {
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {  // Close Window
                mainWindow.close();
                return 0;
            }

            // Press Buttons
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < buttonNames.size(); ++i) {
                    if (buttons[i].PressButton(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        std::string result = buttonFuntions[i]();
                        ReleaseFunctions(result, i, mainWindow, image, fileField, statusBar);
                    }
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                //
            }
        }
        mainWindow.clear();

        // Draw elements

        fileField.DrawField(mainWindow);
        statusBar.DrawStatusBar(mainWindow);  // Status bar

        for (const auto& button : buttons) {  // Buttons
            button.DrawButton(mainWindow);
        }

        for (const auto& buttonIcon : buttonIcons) {  // Buttons icons
            mainWindow.draw(buttonIcon);
        }

        mainWindow.draw(menuShape);       // Small menu
        menuImage.DrawImage(mainWindow);  // Small menu image

        image.DrawImage(mainWindow);

        mainWindow.display();
    }

    return 0;
}
