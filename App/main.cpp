#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
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

auto main(int, char**) -> int {
    sf::RenderWindow mainWindow(sf::VideoMode(kMainWindowWidth, kMainWindowHeight), "Image Editor");

    sf::Font buttonFont;  // Font for Buttons
    if (!buttonFont.loadFromFile("../WindowFiles/open-sans.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    // Interface: current image, field with files names, status bar
    Image image;
    std::stack<sf::Image> previousStatus;  // Image modification history

    FileField fileField;
    StatusBar statusBar;

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

    // Menu Buttons
    const std::vector<std::string> buttonNames{"Add file", "Delete file", "Save file", "Select file", "Brush"};
    std::vector<sf::RectangleShape> buttonIcons = LoadButtonImages();  // Button Icons

    std::vector<Button> buttons;
    const std::vector<sf::Color> buttonColors{kFileButtonColor, kFileButtonColor, kFileButtonColor, kFileButtonColor, kToolsColor, kToolsColor};
    Button::CreateMenuButtons(buttons, buttonNames, buttonColors, buttonFont);

    // Main Button Functions
    ButtonFunction buttonFunctions[]{AddFile, DeleteFile, SaveFile, SelectFile, SelectBrush};

    // Brush
    Brush brush(kBrushInitialRadius, sf::Color::White);
    bool brushPressed{false};

// Small menu for brush: Size, Color
    // Size
    Image brushSizeImage;
    brushSizeImage.LoadImage("../WindowFiles/brush-size.png");
    brushSizeImage.SetScale(kBrushImageScale, kBrushImageScale);
    brushSizeImage.SetPosition(kSmallMenuWidth + 5 * kButtonWidth + kButtonWidth / 10, 0);

    const float kBrushInputFieldPosX{kSmallMenuWidth + 5 * kButtonWidth + 0.9f * brushSizeImage.GetSpriteBound().width + kButtonWidth / 5};
    const float kBrushInputFieldPoxY{(kButtonHeight - kBrushBoxHeight) / 2};

    InputField brushSizeField(kBrushInputFieldPosX, kBrushInputFieldPoxY, sf::Vector2f(kBrushBoxWidth, kBrushBoxHeight));
    brushSizeField.SetBoxColor(kBrushInputSizeColor);

    brushSizeField.SetText(std::to_string(kBrushInitialRadius));
    brushSizeField.SetTextColor(sf::Color::White);
    brushSizeField.SetTextSize(kCharacterSize / 1.1f);

    // Create small menu for brush color and brush size

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
                    if (buttons[i].AimButton(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        std::string result = buttonFunctions[i]();
                        ReleaseFunctions(result, i, mainWindow, image, fileField, statusBar, brushPressed, previousStatus);
                    }
                }
            }

            // Brush drawing
            if (brushPressed && event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (event.mouseMove.x >= kFileFieldWidth && event.mouseMove.y >= kButtonHeight && event.mouseMove.y <= kMainWindowHeight - kStatusBarHeight) {
                    image.SaveState(previousStatus);
                    brush.Draw(image.GetImage(), image.GetImagePosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)));

                    image.UpdateTexture();
                }
            }

            // Change brush size (Increase size)
            if (brushPressed && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RBracket) {
                brush.SetRadius(std::min(brush.GetRadius() + kBrushChangeRadius, 1000));
                brushSizeField.SetText(std::to_string(brush.GetRadius()));

                brush.UpdateCursorScale();
            }

            // Change brush size (Decrease size)
            if (brushPressed && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LBracket) {
                brush.SetRadius(std::max(kBrushInitialRadius / kBrushInitialRadius, brush.GetRadius() - kBrushChangeRadius));
                brushSizeField.SetText(std::to_string(brush.GetRadius()));

                brush.UpdateCursorScale();
            }

            // Return previous image (Ctrl+Z)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                image.BackState(previousStatus);
            }

            // Save image (Ctrl+S)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                std::string result = buttonFunctions[2]();
                ReleaseFunctions(result, 2, mainWindow, image, fileField, statusBar, brushPressed, previousStatus);
            }
        }

        mainWindow.clear();

        // Draw elements
        mainWindow.draw(background);          // Button background
        image.DrawImage(mainWindow);          // Main image
        fileField.DrawField(mainWindow);      // Field with added files
        statusBar.DrawStatusBar(mainWindow);  // Status bar

        mainWindow.draw(menuShape);       // Small menu
        menuImage.DrawImage(mainWindow);  // Small menu image

        size_t activeButton{Button::GetActiveButton()};
        bool buttonTarget{false};

        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].AimButton(static_cast<sf::Vector2f>(sf::Mouse::getPosition(mainWindow)))) {
                buttonTarget = true;
            }

            buttons[i].AnimateButton(mainWindow, i);

            if (i != activeButton) {
                buttons[i].DrawButton(mainWindow);
                mainWindow.draw(buttonIcons[i]);
            }
        }

        if (buttonTarget && activeButton != std::numeric_limits<size_t>::max()) {
            buttons[activeButton].DrawButton(mainWindow);
            mainWindow.draw(buttonIcons[activeButton]);
        }

        if (brushPressed) {
            brush.SetBrushCursor(sf::Mouse::getPosition(mainWindow));
            mainWindow.draw(brush.GetBrushCursor());
        }

        brushSizeImage.DrawImage(mainWindow);
        brushSizeField.Draw(mainWindow);

        mainWindow.display();
    }

    return 0;
}
