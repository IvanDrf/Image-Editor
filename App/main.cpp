#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

// Interface

#include "Brush/Brush.hpp"
#include "Button/Button.hpp"
#include "FileField/FileField.hpp"
#include "Image/Image.hpp"
#include "MainWindow/Interface/Interface.hpp"
#include "MainWindow/MainWindow.hpp"
#include "StatusBar/StatusBar.hpp"

// Menu Buttons, realization in InputField.cpp
namespace Front {
using Paths = const std::vector<std::string>&;

std::string AddFile([[maybe_unused]] Paths pathsToFile, [[maybe_unused]] size_t activeFile);
std::string DeleteFile(Paths pathsToFile, size_t activeFile);

std::string SaveFile(Paths pathsToFile, size_t activeFile);

std::string SelectBrush([[maybe_unused]] Paths pathsToFile, [[maybe_unused]] size_t activeFile);
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

    FileField fileField;
    StatusBar statusBar;

    auto [backgorund, menuShape, menuImage, brushSizeImage] = Interface::CreateInterface();

    // Menu Buttons
    const std::vector<std::string> buttonNames = {"Add file", "Delete file", "Save file", "Brush"};
    std::vector<sf::RectangleShape> buttonIcons = Interface::LoadButtonImages();  // Button Icons

    std::vector<Button> buttons;
    const std::vector<sf::Color> buttonColors{kFileButtonColor, kFileButtonColor, kFileButtonColor, kFileButtonColor, kToolsColor, kToolsColor};
    Button::CreateMenuButtons(buttons, buttonNames, buttonColors, buttonFont);

    // Main Button Functions
    ButtonFunction buttonFunctions[]{Front::AddFile, Front::DeleteFile, Front::SaveFile, Front::SelectBrush};

    // Brush
    Brush brush(kBrushInitialRadius, sf::Color::White);
    bool brushPressed{false};

    // Brush size field, displays current brush size
    // Brush size field position

    auto [brushSizeFieldPosX, brushSizeFieldPosY] = Interface::CalculateBrushSizePos(brushSizeImage);

    BrushSizeDisplay brushSizeField(kBrushInitialRadius, buttonFont);
    brushSizeField.SetPosition(brushSizeFieldPosX, brushSizeFieldPosY);
    brushSizeField.SetShapeSize(sf::Vector2f(kBrushBoxWidth, kBrushBoxHeight));

    // Brush current color, displays current brush color
    BrushColorDisplay brushCurrentColor(kBrushCurrentColorBoxSize);
    brushCurrentColor.SetColor(brush.GetColor());

    // Brush current color shape position
    auto [brushColorShapePosX, brushColorShapePosY] = Interface::CalculateBrushColorShapePos({brushSizeFieldPosX, brushSizeFieldPosY});
    brushCurrentColor.SetPosition(brushColorShapePosX, brushColorShapePosY);

    // Create small menu for brush color and brush size

    std::vector<std::string> pathsToFile;                   // Paths to files
    size_t activeFile{std::numeric_limits<size_t>::max()};  // Current active file
    size_t previousFile{};                                  // Previous active file

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
                        std::string result = buttonFunctions[i](pathsToFile, activeFile);  // Path to file

                        [[maybe_unused]] const size_t oldFilesCount = pathsToFile.size();                                 // Check was file deleted succesfully or not
                        WorkWithPath(pathsToFile, result, i, image, fileField, statusBar, brushPressed, previousStatus);  // Work with main buttons

                        if (image.HasImage() && oldFilesCount != pathsToFile.size()) {
                            Back::SelectNewActiveFile(i, activeFile);
                        }

                        if (brushPressed && buttons[Buttons::SelectBrush].GetColor() != kActiveButtonColor) {
                            buttons[Buttons::SelectBrush].SetColor(kActiveButtonColor);
                        } else if (!brushPressed && buttons[Buttons::SelectBrush].GetColor() != kToolsColor) {
                            buttons[Buttons::SelectBrush].SetColor(kToolsColor);
                        }
                    }
                }

                activeFile = fileField.GetActiveFile(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), activeFile);
                if (activeFile != std::numeric_limits<size_t>::max() && activeFile != previousFile) {
                    try {
                        image.ClearImage(previousStatus);          // Destroy old image
                        image.LoadImage(pathsToFile[activeFile]);  // Load selected image
                        image.SetMainImageScale();                 // Set main properties

                    } catch (std::runtime_error& e) {
                        statusBar.UpdateStatus(e.what());
                    }

                    previousFile = activeFile;
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
                brushSizeField.SetText(brush.GetRadius());
                brush.UpdateCursorScale();
            }

            // Change brush size (Decrease size)
            if (brushPressed && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LBracket) {
                brush.SetRadius(std::max(kBrushInitialRadius / kBrushInitialRadius, brush.GetRadius() - kBrushChangeRadius));
                brushSizeField.SetText(brush.GetRadius());
                brush.UpdateCursorScale();
            }

            // Set Brush Color
            if (brushPressed && event.type == sf::Event::KeyPressed) {
                brush.SetColor(event.key.code, brushCurrentColor);
            }

            // Return previous image (Ctrl+Z)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                image.BackState(previousStatus);
            }

            // Save image (Ctrl+S)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                std::string result = buttonFunctions[2](pathsToFile, activeFile);
                WorkWithPath(pathsToFile, result, 2, image, fileField, statusBar, brushPressed, previousStatus);
            }
        }

        mainWindow.clear();

        // Draw elements
        mainWindow.draw(backgorund);                  // Button backgorund
        image.DrawImage(mainWindow);                  // Main image
        fileField.DrawField(mainWindow, activeFile);  // Field with added files
        statusBar.DrawStatusBar(mainWindow);          // Status bar

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

        brushSizeImage.DrawImage(mainWindow);  // Brush current size image
        brushSizeField.Draw(mainWindow);       // Brush current size field

        brushCurrentColor.Draw(mainWindow);  // Brush current color field

        mainWindow.display();
    }

    return 0;
}
