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
    sf::RenderWindow mainWindow(sf::VideoMode({kMainWindowWidth, kMainWindowHeight}), "Image Editor");
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
        while (auto event = mainWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {  // Close window
                mainWindow.close();
                return 0;
            }

            // Press Buttons
            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                auto mousePos = event->getIf<sf::Event::MouseButtonPressed>();

                for (size_t i = 0; i < buttonNames.size(); ++i) {
                    if (buttons[i].AimButton(mousePos->position)) {
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
                if (brushSizeField.ShapeClicked(mousePos->position)) {
                    brushSizeFieldPressed = true;

                    brush.SetRadius(0);
                    brushSizeField.SetText(0);

                    brush.UpdateCursorScale(currentZoom);
                } else {
                    brushSizeFieldPressed = false;
                }

                // Select active image by mouse
                activeFile = fileField.GetActiveFile(mousePos->position, activeFile);
                ActiveFile::SelectActiveImage(appData);

                // Zoom In
                if (image.HasImage() && zoomInImage->GetSpriteBound().contains(static_cast<sf::Vector2f>(mousePos->position))) {
                    Zoom::ZoomIn(image);
                    currentZoom += kZoomStep;

                    brush.UpdateCursorScale(currentZoom);

                    statusBar.UpdateStatus("Current zoom: " + std::to_string(currentZoom).substr(0, 3));
                }

                // Zoom Out
                if (image.HasImage() && zoomOutImage->GetSpriteBound().contains(static_cast<sf::Vector2f>(mousePos->position))) {
                    if (currentZoom - kZoomStep >= 0) {
                        Zoom::ZoomOut(image);
                        currentZoom = currentZoom - kZoomStep;

                        brush.UpdateCursorScale(currentZoom);

                        statusBar.UpdateStatus("Current zoom: " + std::to_string(currentZoom).substr(0, 3));
                    }
                }
            }

            // Beginning of moving image
            if (image.HasImage() && event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left &&
                (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || isMoved)) {
                isMoved = true;

                brushPressed = false;
                if (buttons[Buttons::SelectBrush].GetColor() == kActiveButtonColor) {
                    buttons[Buttons::SelectBrush].SetColor(kToolsColor);
                    statusBar.UpdateStatus("Brush is no longer selected");
                }

                currentMousePos = sf::Mouse::getPosition(mainWindow);
            }

            // Ending of moving image
            if (image.HasImage() && event->is<sf::Event::MouseButtonReleased>() && !isMoveButton) {
                isMoved = false;
            }

            // Reset image postion -> default image position and zoom
            if (!brushPressed && image.HasImage() && event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                Zoom::Reset(image, statusBar, brush, currentZoom);
            }

            // Select active image by key 'up'
            if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up && activeFile != NONE) {
                ActiveFile::SelectUpperImage(appData);
            }

            // Select active image by key 'down'
            if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down) {
                ActiveFile::SelectLowerImage(appData);
            }

            // Brush drawing
            if (brushPressed && !isPaletteOpen && event->is<sf::Event::MouseMoved>() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                Brush::DrawOnImage(appData, event);
            }

            // Change brush size
            if (brushSizeFieldPressed && event->is<sf::Event::TextEntered>()) {
                Brush::InputBrushSize(appData, event, brushSizeField);  // Input brush size in brush field
            }

            // Change brush size (Increase size)
            if (brushPressed && event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::RBracket) {
                Brush::IncreaseBrushSize(appData, brushSizeField);
            }

            // Change brush size (Decrease size)
            if (brushPressed && event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::LBracket) {
                Brush::DecreaseBrushSize(appData, brushSizeField);
            }

            // Set Brush Color by hot key
            if (brushPressed && event->is<sf::Event::KeyPressed>()) {
                Brush::SetBrushColorKey(brush, brushColorField, event->getIf<sf::Event::KeyPressed>()->code);
            }

            // Set Brush Color by Palette
            if (isPaletteOpen && event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                auto mousePos = event->getIf<sf::Event::MouseButtonPressed>()->position;
                brushColorField.SetPaletteColor(brush, mousePos.x, mousePos.y);
            }

            // Open and close Palette
            if (brushPressed && event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                auto mousePos = event->getIf<sf::Event::MouseButtonPressed>()->position;
                if (brushColorField.ShapeClicked(mousePos)) {
                    isPaletteOpen = !isPaletteOpen;
                }
            }

            // Return previous image (Ctrl+Z)
            if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                image.BackState(previousStatus);
            }

            // Add new image (Ctrl+N)
            if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::O && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                std::string result{Interface::AddFile(pathsToFile, activeFile)};
                Back::ButtonsFunc(appData, Buttons::AddFile);
            }

            // Save image (Ctrl+S)
            if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                std::string result{Interface::SaveFile(pathsToFile, activeFile)};
                Back::ButtonsFunc(appData, Buttons::SaveFile);
            }

            // Delete current file
            if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Delete) {
                std::string result{Interface::DeleteFile(pathsToFile, activeFile)};

                ActiveFile::ChangeActiveFileNumber(Buttons::DeleteFile, activeFile, pathsToFile.size());
                Back::ButtonsFunc(appData, Buttons::DeleteFile);
            }
        }

        // Moving image
        if (image.HasImage() && isMoved && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
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
