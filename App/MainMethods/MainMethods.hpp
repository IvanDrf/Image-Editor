#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../FileField/FileField.hpp"
#include "../Image/Image.hpp"
#include "../StatusBar/StatusBar.hpp"

namespace Front {
std::string AddFile();
std::string SaveFile();
std::string DeleteFile();

std::string SelectBrush();
}  // namespace Front

namespace Back {
void AddFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus);
void DeleteFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& FileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus);

void SaveFile(const std::string& result, Image& image, StatusBar& statusBar);
void SelectFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus);
void SelectBrush(bool& brushPressed, const Image& image, StatusBar& StatusBar);
}  // namespace Back
