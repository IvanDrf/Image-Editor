#include "FileField.hpp"

#include <limits>

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

    activeFileBackGround_.setSize(kFileBackGroundSize);
    activeFileBackGround_.setFillColor(kFileBackGroundColor);
    activeFileBackGround_.setPosition(0, kButtonHeight);
    // activeFileBackGround_.setOrigin(0, kFileBackGroundSize.y / 2);
}

void FileField::DrawField(sf::RenderWindow& window, size_t activeFile) {
    window.draw(shape_);

    displayedFiles_.clear();

    sf::Text filesNames;
    filesNames.setFont(font_);
    filesNames.setCharacterSize(kCharacterSize);
    filesNames.setFillColor(sf::Color::White);

    float textPositionY{1.15f * kButtonHeight};

    activeFileBackGround_.setPosition(0, textPositionY + activeFile * kLineHeight);
    if (!files_.empty()) {
        window.draw(activeFileBackGround_);
    }

    for (const auto& fileName : files_) {
        filesNames.setString(fileName);

        filesNames.setPosition(10, textPositionY);
        textPositionY += kLineHeight;

        displayedFiles_.emplace_back(filesNames);
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

size_t FileField::GetActiveFile(const sf::Vector2i& mousePosition, size_t activeFile) const {
    if (files_.size() == 1) {
        return 0;
    }

    for (size_t i = 0; i < displayedFiles_.size(); ++i) {
        if (displayedFiles_[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
            return i;
        }
    }

    return activeFile;
}
