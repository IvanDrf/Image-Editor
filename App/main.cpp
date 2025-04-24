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

#define NONE (std::numeric_limits<std::size_t>::max())

// Menu Buttons, realization in InputField.cpp
namespace Front {
using Paths = const std::vector<std::string>;

std::string AddFile([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile);
std::string DeleteFile(Paths& pathsToFile, size_t activeFile);

std::string SaveFile(Paths& pathsToFile, size_t activeFile);

std::string SelectBrush([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile);
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
    Button::CreateFileButtons(buttons, buttonNames, buttonColors, buttonFont);

    // Main Button Functions
    ButtonFunction buttonFunctions[]{Front::AddFile, Front::DeleteFile, Front::SaveFile, Front::SelectBrush};

    // Brush
    Brush brush(kBrushInitialRadius, sf::Color::White);
    bool brushPressed{false};
    bool isPaletteOpen{false};

    // Brush size field, displays current brush size
    // Brush size field position

    auto [brushSizeFieldPosX, brushSizeFieldPosY] = Interface::CalculateBrushSizePos(brushSizeImage);

    BrushSizeDisplay brushSizeField(kBrushInitialRadius, buttonFont);
    bool brushSizeFieldPressed{false};

    brushSizeField.SetPosition(brushSizeFieldPosX, brushSizeFieldPosY);
    brushSizeField.SetShapeSize({kBrushBoxWidth, kBrushBoxHeight});

    // Brush current color, displays current brush color
    BrushColorDisplay brushCurrentColor(kBrushCurrentColorBoxSize);
    brushCurrentColor.SetColor(brush.GetColor());

    auto [palettePosX, palettePosY] = Interface::CalculatePalettePos({brushSizeFieldPosX, kButtonHeight});
    brushCurrentColor.SetPalettePosition(palettePosX, palettePosY);

    // Brush current color shape position
    auto [brushColorShapePosX, brushColorShapePosY] = Interface::CalculateBrushColorShapePos({brushSizeFieldPosX, brushSizeFieldPosY});
    brushCurrentColor.SetPosition(brushColorShapePosX, brushColorShapePosY);

    // Zoom
    auto [zoomOut, zoomIn, zoomBackground] = Interface::LoadZoomImages();

    // Create small menu for brush color and brush size

    std::vector<std::string> pathsToFile;  // Paths to files
    size_t activeFile{NONE};               // Current active file
    size_t previousFile{};                 // Previous active file

    ActiveFile::ActiveContext activeContext{activeFile, previousFile, pathsToFile, image, previousStatus, statusBar};

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
                    if (buttons[i].AimButton({event.mouseButton.x, event.mouseButton.y})) {
                        std::string result = buttonFunctions[i](pathsToFile, activeFile);  // Path to file

                        [[maybe_unused]] const size_t oldFilesCount = pathsToFile.size();                                             // Check was file deleted succesfully or not
                        WorkWithPath(pathsToFile, activeFile, result, i, image, fileField, statusBar, brushPressed, previousStatus);  // Work with main buttons

                        if (image.HasImage() && oldFilesCount != pathsToFile.size()) {
                            Back::SelectNewActiveFile(i, activeFile, pathsToFile.size());
                        }

                        if (brushPressed && buttons[Buttons::SelectBrush].GetColor() != kActiveButtonColor) {
                            buttons[Buttons::SelectBrush].SetColor(kActiveButtonColor);
                        } else if (!brushPressed && buttons[Buttons::SelectBrush].GetColor() != kToolsColor) {
                            buttons[Buttons::SelectBrush].SetColor(kToolsColor);
                        }
                    }
                }

                // If brush size field has been pressed -> change brush size
                if (brushSizeField.ShapeClicked({event.mouseButton.x, event.mouseButton.y})) {
                    brushSizeFieldPressed = true;

                    brush.SetRadius(0);
                    brushSizeField.SetText(0);

                    brush.UpdateCursorScale();
                } else {
                    brushSizeFieldPressed = false;
                }

                // Select active image by mouse
                activeFile = fileField.GetActiveFile({event.mouseButton.x, event.mouseButton.y}, activeFile);
                ActiveFile::SelectActiveImage(activeContext);

                const sf::Vector2i mousePosition{event.mouseButton.x, event.mouseButton.y};
                if (image.HasImage() && zoomIn->GetSpriteBound().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    Zoom::ZoomIn(image);
                }

                if (image.HasImage() && zoomOut->GetSpriteBound().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    Zoom::ZoomOut(image);
                }
            }

            // Select active image by key 'up'
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up && activeFile != NONE) {
                ActiveFile::SelectUpperImage(activeContext);
            }

            // Select active image by key 'down'
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                ActiveFile::SelectLowerImage(activeContext);
            }

            // Brush drawing
            if (brushPressed && !isPaletteOpen && event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                const sf::Vector2f imageBoundary{kFileFieldWidth + image.GetSpriteBound().width, kButtonWidth + image.GetSpriteBound().height};

                if (event.mouseMove.x >= kFileFieldWidth && event.mouseMove.x <= imageBoundary.x && event.mouseMove.y >= kButtonHeight && event.mouseMove.y <= imageBoundary.y) {
                    image.SaveState(previousStatus);
                    brush.Draw(image.GetImage(), image.GetImagePosition({event.mouseMove.x, event.mouseMove.y}));

                    image.UpdateTexture();
                }
            }

            // Change brush size
            if (brushSizeFieldPressed && event.type == sf::Event::TextEntered) {
                brush.SetRadius(brushSizeField.InputSize(event));
                brushSizeField.SetText(brush.GetRadius());

                brush.UpdateCursorScale();
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

            // Set Brush Color by hot key
            if (brushPressed && event.type == sf::Event::KeyPressed) {
                brush.SetColor(event.key.code, brushCurrentColor);
                brushCurrentColor.SetColor(brush.GetColor());
            }

            // Set Brush Color by Palette
            if (isPaletteOpen && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (brushCurrentColor.PaletteClicked({event.mouseButton.x, event.mouseButton.y})) {
                    brush.SetColor(brushCurrentColor.GetPaletteColor({event.mouseButton.x, event.mouseButton.y}));

                    brushCurrentColor.SetColor(brush.GetColor());
                }
            }

            // Open and close Palette
            if (brushPressed && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (brushCurrentColor.ShapeClicked({event.mouseButton.x, event.mouseButton.y})) {
                    isPaletteOpen = !isPaletteOpen;
                }
            }

            // Return previous image (Ctrl+Z)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                image.BackState(previousStatus);
            }

            // Add new image (Ctrl+N)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                std::string result = Front::AddFile(pathsToFile, activeFile);
                WorkWithPath(pathsToFile, activeFile, result, Buttons::AddFile, image, fileField, statusBar, brushPressed, previousStatus);
            }

            // Save image (Ctrl+S)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                std::string result = Front::SaveFile(pathsToFile, activeFile);
                WorkWithPath(pathsToFile, activeFile, result, Buttons::SaveFile, image, fileField, statusBar, brushPressed, previousStatus);
            }

            // Delete current file
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete) {
                std::string result = Front::DeleteFile(pathsToFile, activeFile);

                Back::SelectNewActiveFile(Buttons::DeleteFile, activeFile, pathsToFile.size());
                WorkWithPath(pathsToFile, activeFile, result, Buttons::DeleteFile, image, fileField, statusBar, brushPressed, previousStatus);
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
            if (buttons[i].AimButton(sf::Mouse::getPosition(mainWindow))) {
                buttonTarget = true;
            }

            buttons[i].AnimateButton(mainWindow, i);

            if (i != activeButton) {
                buttons[i].DrawButton(mainWindow);
                mainWindow.draw(buttonIcons[i]);
            }
        }

        if (buttonTarget && activeButton != NONE) {
            buttons[activeButton].DrawButton(mainWindow);
            mainWindow.draw(buttonIcons[activeButton]);
        }

        if (brushPressed) {  // Draw brush cursor
            brush.SetBrushCursor(sf::Mouse::getPosition(mainWindow));
            mainWindow.draw(brush.GetBrushCursor());
        }

        brushSizeImage.DrawImage(mainWindow);  // Brush current size image
        brushSizeField.Draw(mainWindow);       // Brush current size field

        brushCurrentColor.Draw(mainWindow);  // Brush current color field

        if (isPaletteOpen) {
            brushCurrentColor.DrawPalette(mainWindow);  // Draw palette
        }

        if (!brushPressed) {
            zoomBackground->DrawImage(mainWindow);
            zoomIn->DrawImage(mainWindow);
            zoomOut->DrawImage(mainWindow);
        }

        mainWindow.display();
    }

    return 0;
}
