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

// Menu Buttons
std::string AddFile();
std::string DeleteFile();
std::string SaveFile();

auto main(int, char**) -> int {
    sf::RenderWindow mainWindow(sf::VideoMode(kMainWindowWidth, kMainWindowHeight), "Image Editor");

    // Interface: current image, field with files names, status bar
    Image image;
    FileField fileField;
    StatusBar statusBar;

    sf::Font mainFont;  // Font for Buttons
    if (!mainFont.loadFromFile("../WindowFiles/open-sans.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    // Menu Buttons
    const std::vector<std::string> buttonNames{"Add file", "Delete file", "Save file", "Select File"};
    std::vector<sf::Sprite> buttonIcons;
    LoadButtonImages(buttonIcons);

    std::vector<Button> buttons;

    Button::CreateMenuButtons(buttons, buttonNames, mainFont);
    ButtonFunction buttonFuntions[]{AddFile, DeleteFile, SaveFile, SelectFile};

    // Main Loop
    while (mainWindow.isOpen()) {
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mainWindow.close();
                return 0;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < buttonNames.size(); ++i) {
                    if (buttons[i].PressButton(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        std::string result = buttonFuntions[i]();
                        ReleaseFunctions(result, i, mainWindow, image, fileField, statusBar);
                    }
                }
            }

            mainWindow.clear();

            fileField.DrawField(mainWindow);

            statusBar.DrawStatusBar(mainWindow);
            for (const auto& button : buttons) {
                button.DrawButton(mainWindow);
            }

            for (const auto& buttonIcon : buttonIcons) {
                mainWindow.draw(buttonIcon);
            }

            image.DrawImage(mainWindow);

            mainWindow.display();
        }
    }

    return 0;
}
