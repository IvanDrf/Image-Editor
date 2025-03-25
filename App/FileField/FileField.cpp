#include "FileField.hpp"

#include "../MainWindow/MainWindow.hpp"

FileField::FileField() {
    if (!font_.loadFromFile("../WindowFiles/open-sans-semibold.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    shape_.setSize(sf::Vector2f(kFileFieldWidth, kFileFieldHeight));
    shape_.setFillColor(KSystemColor);
    shape_.setOutlineColor(sf::Color::Black);
    shape_.setOutlineThickness(kDefaultOutlineThickness);

    shape_.setPosition(0, kMainWindowHeight - kFileFieldHeight);
}

void FileField::DrawField(sf::RenderWindow& window) {
    window.draw(shape_);

    sf::Text filesNames;
    filesNames.setFont(font_);
    filesNames.setCharacterSize(kCharacterSize);
    filesNames.setFillColor(sf::Color::White);

    float distanceBetweenFiles{kButtonHeight};
    for (const auto& fileName : files_) {
        filesNames.setString(fileName);

        filesNames.setPosition(10, distanceBetweenFiles);
        distanceBetweenFiles += 30;

        window.draw(filesNames);
    }
}

void FileField::AddFile(const std::string& fileName) {
    files_.emplace_back(fileName);
}

void FileField::DeleteFile(const std::string& fileName) {
    if (std::find(files_.begin(), files_.end(), fileName) == files_.end()) {
        return;
    }

    files_.erase(std::remove(files_.begin(), files_.end(), fileName), files_.end());
}

const std::vector<std::string>& FileField::GetFiles() const {
    return files_;
}
