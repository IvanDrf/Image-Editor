#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

// Interface
#include <gtk/gtk.h>

#include <iostream>

#include "Brush/Brush.hpp"
#include "Button/Button.hpp"
#include "FileField/FileField.hpp"
#include "Image/Image.hpp"
#include "MainMethods/MainMethods.hpp"
#include "MainWindow/MainWindow.hpp"
#include "StatusBar/StatusBar.hpp"

namespace Front {
std::string AddFile();
std::string DeleteFile();
std::string SelectBrush();
}  // namespace Front

auto main(int, char**) -> int {
    sf::RenderWindow mainWindow(sf::VideoMode(kMainWindowWidth, kMainWindowHeight), "Image Editor");
    mainWindow.setFramerateLimit(30);

    sf::Font buttonFont;  // Font for Buttons
    if (!buttonFont.loadFromFile("../WindowFiles/open-sans.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    // Interface: current image, field with files names, status bar
    Image image;
    std::stack<sf::Image> previousStatus;  // Image modification history

    std::vector<std::string> pathToFile;  // Vector with paths -> files
    FileField fileField;                  // Field with files names
    StatusBar statusBar;                  // Displays the current status

    sf::RectangleShape background;  // Background behind buttons and brush
    background.setFillColor(kToolsColor);
    background.setSize(sf::Vector2f(kMainWindowWidth, kSmallMenuHeight));
    background.setPosition(0, 0);

    // Small menu
    sf::RectangleShape menuShape;  // Small menu (unused) in upper left conner
    menuShape.setFillColor(kToolsColor);
    menuShape.setSize(sf::Vector2f(kSmallMenuWidth, kSmallMenuHeight));
    menuShape.setPosition(0, 0);

    menuShape.setOutlineColor(sf::Color::Black);
    menuShape.setOutlineThickness(3);

    Image menuImage;  // Image for small menu
    menuImage.LoadImage("../WindowFiles/smallMenu-image.png");
    menuImage.SetScale(kSmallMenuScale, kSmallMenuScale);
    menuImage.SetOrigin(menuImage.GetSpriteBound().width / 2, 0);
    menuImage.SetPosition(kSmallMenuIconPosition);

    // Menu Buttons
    const std::vector<std::string> buttonNames{"Add file", "Delete file", "Save file", "Brush"};
    std::vector<sf::RectangleShape> buttonIcons = LoadButtonImages();  // Button Icons

    std::vector<Button> buttons;
    const std::vector<sf::Color> buttonColors{kFileButtonColor, kFileButtonColor, kFileButtonColor, kFileButtonColor, kToolsColor, kToolsColor};
    Button::CreateMenuButtons(buttons, buttonNames, buttonColors, buttonFont);
    buttons.back().SetColor(kToolsColor);

    // Main Button Functions
    ButtonFunction buttonFunctions[]{Front::AddFile, Front::SaveFile, Front::DeleteFile, Front::SelectBrush};

    // Brush
    Brush brush(kBrushInitialRadius, sf::Color::White);
    bool brushPressed{false};

    // Small menu for brush: Size, Color
    // Size
    Image brushSizeImage;
    brushSizeImage.LoadImage("../WindowFiles/brush-size.png");
    brushSizeImage.SetScale(kBrushImageScale, kBrushImageScale);
    brushSizeImage.SetPosition(kBrushSizeImagePosition);

    const float kBrushSizeFieldPosX{kSmallMenuWidth + 4 * kButtonWidth + 0.9f * brushSizeImage.GetSpriteBound().width + kButtonWidth / 5};
    const float kBrushSizeFieldPoxY{(kButtonHeight - kBrushBoxHeight) / 2};

    InputField brushSizeField(kBrushSizeFieldPosX, kBrushSizeFieldPoxY, sf::Vector2f(kBrushBoxWidth, kBrushBoxHeight));
    brushSizeField.SetBoxColor(kBrushInputSizeColor);

    brushSizeField.SetText(std::to_string(kBrushInitialRadius));
    brushSizeField.SetTextColor(sf::Color::White);
    brushSizeField.SetTextSize(kBrushSizeFieldCharacterSize);

    sf::RectangleShape brushCurrentColorShape;
    brushCurrentColorShape.setFillColor(brush.GetColor());
    brushCurrentColorShape.setSize(kBrushCurrentColorBoxSize);

    const sf::Vector2f kBrushColorShapePosition(brushSizeField.GetPosition().x + 1.35f * kBrushBoxWidth, brushSizeField.GetPosition().y);
    brushCurrentColorShape.setPosition(kBrushColorShapePosition);

    brushCurrentColorShape.setOutlineColor(sf::Color::White);
    brushCurrentColorShape.setOutlineThickness(kDefaultOutlineThickness);

    // Create small menu for brush color and brush size

    size_t activeFile{};    // Current index of active file
    size_t previousFile{};  // Previous loaded file

    // Main Loop
    while (mainWindow.isOpen()) {
        while (gtk_events_pending()) {
            gtk_main_iteration();
        }

        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {  // Close Window
                mainWindow.close();
                return 0;
            }

            // Press Buttons
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < buttonNames.size(); ++i) {
                    if (buttons[i].AimButton(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) {
                        std::string result = buttonFunctions[i]();
                        ReleaseFunctions(pathToFile, result, i, image, fileField, statusBar, brushPressed, previousStatus);
                    }
                }

                if (brushPressed && buttons[Buttons::SelectBrush].GetColor() != kActiveButtonColor) {
                    buttons[Buttons::SelectBrush].SetColor(kActiveButtonColor);
                } else if (!brushPressed && buttons[Buttons::SelectBrush].GetColor() != kToolsColor) {
                    buttons[Buttons::SelectBrush].SetColor(kToolsColor);
                }

                activeFile = fileField.GetActiveFile(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            }

            // Select file
            if (activeFile != std::numeric_limits<size_t>::max() && !pathToFile.empty() && activeFile != previousFile) {
                std::cout << "0" << '\n';
                image.LoadImage(pathToFile[activeFile]);  // Load new active file
                previousFile = activeFile;
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

            // Set Brush Color
            if (brushPressed && event.type == sf::Event::KeyPressed) {
                brush.SetColor(event.key.code, brushCurrentColorShape);
            }

            // Return previous image (Ctrl+Z)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                image.BackState(previousStatus);
            }

            // Save image (Ctrl+S)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                std::string result = buttonFunctions[2]();
                ReleaseFunctions(pathToFile, result, 2, image, fileField, statusBar, brushPressed, previousStatus);
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
        std::cout << "2" << '\n';

        size_t activeButton{Button::GetActiveButton()};
        bool buttonTarget{false};

        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].AimButton(sf::Mouse::getPosition(mainWindow))) {
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

        brushSizeImage.DrawImage(mainWindow);  // Brush current size image
        brushSizeField.Draw(mainWindow);       // Brush current size field

        mainWindow.draw(brushCurrentColorShape);
        std::cout << "3" << '\n';
        mainWindow.display();
        std::cout << "4" << '\n';
    }

    return 0;
}
