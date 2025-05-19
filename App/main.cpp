#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

#include "Back/Back.hpp"
#include "Brush/Brush.hpp"
#include "Button/Button.hpp"
#include "FileField/FileField.hpp"
#include "Image/Image.hpp"
#include "MainWindow/Interface/Interface.hpp"
#include "MainWindow/MainWindow.hpp"
#include "StatusBar/StatusBar.hpp"

// Menu Buttons, realization in Interface.cpp
namespace Interface {
// File buttons
std::string AddFile(NO_ARGS);
std::string DeleteFile(Paths& pathsToFile, size_t activeFile);
std::string SaveFile(Paths& pathsToFile, size_t activeFile);

// Tools buttons
std::string SelectBrush(NO_ARGS);
std::string MoveImage(NO_ARGS);
std::string Reset(NO_ARGS);
}  // namespace Interface

auto main(int, char**) -> int {
    sf::RenderWindow mainWindow(sf::VideoMode(kMainWindowWidth, kMainWindowHeight), "Image Editor");
    mainWindow.setFramerateLimit(FPS);

    auto mainFont{Interface::LoadMainFont()};

    // Interface: current image, field with files names, status bar
    Image image;
    StackImage previousStatus;  // Image modification history

    FileField fileField;
    StatusBar statusBar;

    auto [background, menuImage, brushSizeImage]{Interface::CreateInterface()};

    // Menu Buttons
    const auto [_, buttonIcons]{Interface::LoadButtonImages()};  // Button Icons

    const NAMES buttonNames = BUTTONS_NAMES;
    BUTTONS buttons{Interface::CreateMenuButtons(buttonNames, BUTTONS_COLORS, mainFont)};

    // Main Button Functions
    ButtonFunction buttonFunctions[] BUTTONS_FUNCTIONS;

    // Brush
    Brush brush{};
    bool brushPressed{false};
    bool isPaletteOpen{false};

    auto brushSizeField{Interface::CreateBrushSizeDisplay(brushSizeImage, mainFont)};  // Shows current brush size
    auto brushColorField{Interface::CreateBrushColorDisplay(brushSizeImage, brush)};   // Shows current brush color

    bool brushSizeFieldPressed{false};

    // Zoom
    auto [zoomOutImage, zoomInImage, zoomBackground]{Interface::LoadZoomImages()};
    float currentZoom{kDefaultZoom};  // Curent zoom image

    bool isMoved{false};             // Is the image being moved
    bool isMoveButton{false};        // Was move button pressed
    sf::Vector2i currentMousePos{};  // Current mouse position in moving image

    // Create small menu for brush color and brush size

    Paths pathsToFile;        // Paths to files
    size_t activeFile{NONE};  // Current active file
    size_t previousFile{};    // Previous active file

    std::string buttonInputResult{};
    AppData appData{pathsToFile, previousFile, activeFile, buttonInputResult, image, currentZoom, fileField, statusBar, previousStatus, brush, brushPressed};

    // Main Loop
    while (mainWindow.isOpen()) {
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {  // Close window
                mainWindow.close();
                return 0;
            }

            // Press Buttons
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < buttonNames.size(); ++i) {
                    if (buttons[i].AimButton({event.mouseButton.x, event.mouseButton.y})) {
                        // Front
                        buttonInputResult = buttonFunctions[i](pathsToFile, activeFile);  // Path to file

                        // Back
                        Back::ButtonsFunc(appData, i);  // Work with main buttons

                        // Move button
                        if (image.HasImage() && i == Buttons::Move) {
                            isMoved = !isMoved;
                            isMoveButton = !isMoveButton;
                            brushPressed = false;  // Change status after swap brush/move

                            currentMousePos = sf::Mouse::getPosition(mainWindow);

                            buttons[i].SetColor((isMoveButton) ? (kActiveButtonColor) : kToolsColor);  // Set active 'Move' button color
                            statusBar.UpdateStatus((isMoveButton) ? ("Move selected") : ("Move is no longer selected"), (isMoved) ? (sf::Color::Green) : (sf::Color::Red));
                        }

                        if (brushPressed && buttons[Buttons::SelectBrush].GetColor() != kActiveButtonColor) {
                            buttons[Buttons::SelectBrush].SetColor(kActiveButtonColor);

                            brushPressed = true;  // Change status after swap move/brush

                            isMoveButton = false;  // Change status after swap move/brush
                            isMoved = false;       // Change status after swap move/brush

                            if (buttons[Buttons::Move].GetColor() != kToolsColor) {
                                buttons[Buttons::Move].SetColor(kToolsColor);  // Reset color for 'Move' button
                            }

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

                    brush.UpdateCursorScale(currentZoom);
                } else {
                    brushSizeFieldPressed = false;
                }

                // Select active image by mouse

                activeFile = fileField.GetActiveFile({event.mouseButton.x, event.mouseButton.y}, activeFile);
                ActiveFile::SelectActiveImage(appData);

                // Zoom In
                const sf::Vector2i mousePosition{event.mouseButton.x, event.mouseButton.y};
                if (image.HasImage() && zoomInImage->GetSpriteBound().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    Zoom::ZoomIn(image);
                    currentZoom += kZoomStep;

                    brush.UpdateCursorScale(currentZoom);

                    statusBar.UpdateStatus("Current zoom: " + std::to_string(currentZoom).substr(0, 3));
                }

                // Zoom Out
                if (image.HasImage() && zoomOutImage->GetSpriteBound().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    if (currentZoom - kZoomStep >= 0) {
                        Zoom::ZoomOut(image);
                        currentZoom = currentZoom - kZoomStep;

                        brush.UpdateCursorScale(currentZoom);

                        statusBar.UpdateStatus("Current zoom: " + std::to_string(currentZoom).substr(0, 3));
                    }
                }
            }

            // Beginning of moving image
            if (image.HasImage() && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && (KEY_PRESSED(sf::Keyboard::Space) || isMoved)) {
                isMoved = true;

                brushPressed = false;
                if (buttons[Buttons::SelectBrush].GetColor() == kActiveButtonColor) {
                    buttons[Buttons::SelectBrush].SetColor(kToolsColor);

                    statusBar.UpdateStatus("Brush is no longer selected");
                }

                currentMousePos = sf::Mouse::getPosition(mainWindow);
            }

            // Ending of moving image
            if (image.HasImage() && event.type == sf::Event::MouseButtonReleased && !isMoveButton) {
                isMoved = false;
            }

            // Reset image postion -> default image position and zoom
            if (!brushPressed && image.HasImage() && event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::R) {
                Zoom::Reset(image, statusBar, brush, currentZoom);
            }

            // Select active image by key 'up'
            if (event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::Up && activeFile != NONE) {
                ActiveFile::SelectUpperImage(appData);
            }

            // Select active image by key 'down'
            if (event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::Down) {
                ActiveFile::SelectLowerImage(appData);
            }

            // Brush drawing
            if (brushPressed && !isPaletteOpen && event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                Brush::DrawOnImage(appData, event);
            }

            // Change brush size
            if (brushSizeFieldPressed && event.type == sf::Event::TextEntered) {
                Brush::InputBrushSize(appData, event, brushSizeField);  // Input brush size in brush field
            }

            // Change brush size (Increase size)
            if (brushPressed && event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::RBracket) {
                Brush::IncreaseBrushSize(appData, brushSizeField);
            }

            // Change brush size (Decrease size)
            if (brushPressed && event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::LBracket) {
                Brush::DecreaseBrushSize(appData, brushSizeField);
            }

            // Set Brush Color by hot key
            if (brushPressed && event.type == sf::Event::KeyPressed) {
                Brush::SetBrushColorKey(brush, brushColorField, KEY);
            }

            // Set Brush Color by Palette
            if (isPaletteOpen && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                brushColorField.SetPaletteColor(brush, event.mouseButton.x, event.mouseButton.y);
            }

            // Open and close Palette
            if (brushPressed && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (brushColorField.ShapeClicked({event.mouseButton.x, event.mouseButton.y})) {
                    isPaletteOpen = !isPaletteOpen;
                }
            }

            // Return previous image (Ctrl+Z)
            if (event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::Z && KEY_PRESSED(sf::Keyboard::LControl)) {
                image.BackState(previousStatus);
            }

            // Add new image (Ctrl+N)
            if (event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::O && KEY_PRESSED(sf::Keyboard::LControl)) {
                std::string result{Interface::AddFile(pathsToFile, activeFile)};
                Back::ButtonsFunc(appData, Buttons::AddFile);
            }

            // Save image (Ctrl+S)
            if (event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::S && KEY_PRESSED(sf::Keyboard::LControl)) {
                std::string result{Interface::SaveFile(pathsToFile, activeFile)};
                Back::ButtonsFunc(appData, Buttons::SaveFile);
            }

            // Delete current file
            if (event.type == sf::Event::KeyPressed && KEY == sf::Keyboard::Delete) {
                std::string result{Interface::DeleteFile(pathsToFile, activeFile)};

                ActiveFile::ChangeActiveFileNumber(Buttons::DeleteFile, activeFile, pathsToFile.size());
                Back::ButtonsFunc(appData, Buttons::DeleteFile);
            }
        }

        // Moving image
        if (image.HasImage() && isMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i lastMousePos{sf::Mouse::getPosition(mainWindow)};

            image.SetPosition(image.GetSprite().getPosition() + sf::Vector2f(lastMousePos - currentMousePos));
            currentMousePos = lastMousePos;
        }

        mainWindow.clear();

        // Draw elements
        image.DrawImage(mainWindow);  // Main image

        fileField.DrawField(mainWindow, activeFile);  // Field with added files
        statusBar.DrawStatusBar(mainWindow);          // Status bar

        mainWindow.draw(background);      // Button background
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

        brushSizeImage.DrawImage(mainWindow);  // Brush size image, circles
        brushSizeField.Draw(mainWindow);       // Brush current size field

        brushColorField.Draw(mainWindow);  // Brush current color field

        if (isPaletteOpen) {
            brushColorField.DrawPalette(mainWindow);  // Draw palette
        }

        if (!brushPressed) {
            zoomBackground->DrawImage(mainWindow);
            zoomInImage->DrawImage(mainWindow);
            zoomOutImage->DrawImage(mainWindow);
        }

        mainWindow.display();
    }

    return 0;
}
