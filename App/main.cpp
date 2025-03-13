#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

// Interface
#include "Brush/Brush.hpp"
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
    //

    sf::RectangleShape background;
    background.setFillColor(kToolsColor);
    background.setSize(sf::Vector2f(kMainWindowWidth, kSmallMenuHeight));
    background.setPosition(0, 0);

    // Small menu
    sf::RectangleShape menuShape;
    menuShape.setFillColor(kToolsColor);
    menuShape.setSize(sf::Vector2f(kSmallMenuWidth, kSmallMenuHeight));
    menuShape.setPosition(0, 0);
    menuShape.setOutlineColor(sf::Color::Black);
    menuShape.setOutlineThickness(3);

    Image menuImage;
    menuImage.LoadImage("../WindowFiles/smallMenu-image.png");
    menuImage.SetScale(kSmallMenuScale, kSmallMenuScale);
    menuImage.SetOrigin(menuImage.GetSpriteBound().width / 2, 0);
    menuImage.SetPosition(kSmallMenuIconPosition);
    //

    // Menu Buttons
    const std::vector<std::string> buttonNames{"Add file", "Delete file", "Save file", " Select file", "Brush"};
    std::vector<sf::RectangleShape> buttonIcons = LoadButtonImages();  // Button Icons

    std::vector<Button> buttons;                                                                                                                  // Vector with buttons
    const std::vector<sf::Color> buttonColors{kFileButtonColor, kFileButtonColor, kFileButtonColor, kFileButtonColor, kToolsColor, kToolsColor};  // Vector with buttons colors
    Button::CreateMenuButtons(buttons, buttonNames, buttonColors, buttonFont);
    //

    // Main Button Functions
    ButtonFunction buttonFuntions[]{AddFile, DeleteFile, SaveFile, SelectFile, SelectBrush};

    // Brush
    Brush brush(kBrushInitialRadius, sf::Color::White);
    bool brushPressed{false};

    brush.Image::LoadImage("../WindowFiles/brush-size.png");
    brush.Image::SetScale(kBrushImageScale, kBrushImageScale);
    brush.Image::SetPosition(kSmallMenuWidth + 5 * kButtonWidth + kButtonWidth / 10, 0);

    brush.InputField::SetBoxSize(sf::Vector2f(kBrushBoxWidth, kBrushBoxHeight));
    brush.InputField::SetBoxPosition(kSmallMenuWidth + 5 * kButtonWidth + 0.9f * brush.Image::GetSpriteBound().width + kButtonWidth / 5, (kButtonHeight - kBrushBoxHeight) / 2);

    std::vector<sf::CircleShape> brushPoints;  // All brush points

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
                        ReleaseFunctions(result, i, mainWindow, image, fileField, statusBar, brushPressed);
                    }
                }
            }

            if (brushPressed && event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::CircleShape point(brush.GetRadius() / 2.0f);
                point.setFillColor(brush.GetColor());

                point.setPosition(event.mouseMove.x - brush.GetRadius() / 2.0f, event.mouseMove.y - brush.GetRadius() / 2.0f);

                brushPoints.push_back(point);
            }

            if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                //
            }
        }
        mainWindow.clear();

        // Draw elements

        mainWindow.draw(background);  // Button backgorund

        image.DrawImage(mainWindow);  // Main image

        fileField.DrawField(mainWindow);      // Field with added files
        statusBar.DrawStatusBar(mainWindow);  // Status bar

        for (const auto& button : buttons) {  // Buttons
            button.DrawButton(mainWindow);
        }

        for (const auto& icon : buttonIcons) {
            mainWindow.draw(icon);
        }

        brush.Image::DrawImage(mainWindow);
        brush.InputField::Draw(mainWindow);

        if (!brushPoints.empty()) {
            for (const auto& point : brushPoints) {  // Draw the dots that were drawn with a brush
                mainWindow.draw(point);
            }
        }
        mainWindow.draw(menuShape);       // Small menu
        menuImage.DrawImage(mainWindow);  // Small menu image

        mainWindow.display();
    }

    return 0;
}
