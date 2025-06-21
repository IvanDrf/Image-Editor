#include "FileField.hpp"

#include <algorithm>
#include <limits>

#include "../MainWindow/MainWindow.hpp"

FileField::FileField() {
    if (!font_.openFromFile("../WindowFiles/Fonts/open-sans-semibold.ttf")) {
        throw std::runtime_error("Font could not be uploaded");
    }

    shape_.setSize({kFileFieldWidth, kFileFieldHeight});
    shape_.setFillColor(KSystemColor);
    shape_.setOutlineColor(sf::Color::Black);
    shape_.setOutlineThickness(kDefaultOutlineThickness);

    shape_.setPosition({0, kMainWindowHeight - kFileFieldHeight});

    activeFileBackGround_.setSize(kFileBackGroundSize);
    activeFileBackGround_.setFillColor(kFileBackGroundColor);
    activeFileBackGround_.setPosition({0, kButtonHeight});
    // appDataFileBackGround_.setOrigin(0, kFileBackGroundSize.y / 2);
}

void FileField::DrawField(sf::RenderWindow& window, size_t activeFile) {
    window.draw(shape_);

    displayedFiles_.clear();

    sf::Text filesNames(font_);
    filesNames.setCharacterSize(kCharacterSize);
    filesNames.setFillColor(sf::Color::White);

    float textPositionY{1.15f * kButtonHeight};

    activeFileBackGround_.setPosition({0, textPositionY + activeFile * kLineHeight});
    if (!files_.empty()) {
        window.draw(activeFileBackGround_);
    }

    for (const auto& fileName : files_) {
        filesNames.setString(fileName);

        filesNames.setPosition({10, textPositionY});
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

const NAMES& FileField::GetFiles() const {
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

namespace {
void UpdateActiveImage(AppData& appData) {
    try {
        appData.image.ClearImage(appData.previousStatus);                  // Destroy old image
        appData.image.LoadImage(appData.pathsToFile[appData.activeFile]);  // Load selected image
        appData.image.SetMainImageScale();                                 // Set main properties
    } catch (std::runtime_error& e) {
        appData.statusBar.UpdateStatus(e.what());
    }

    appData.previousFile = appData.activeFile;
}
}  // namespace

namespace ActiveFile {
void ChangeActiveFileNumber(size_t buttonNumber, size_t& activeFile, size_t files) {
    if (buttonNumber == Buttons::AddFile) {
        activeFile = files - 1;
        return;
    }

    if (buttonNumber == Buttons::DeleteFile) {
        --activeFile;
        if (activeFile == NONE && files > 0) {
            activeFile = 0;
        }
    }
}

void SelectActiveImage(AppData& appData) {
    if (appData.activeFile != NONE && appData.activeFile != appData.previousFile) {
        UpdateActiveImage(appData);
    }
}

void SelectUpperImage(AppData& appData) {
    if (appData.pathsToFile.size() > 1 && appData.activeFile > 0) {
        --appData.activeFile;

        UpdateActiveImage(appData);
    }
}

void SelectLowerImage(AppData& appData) {
    if (appData.pathsToFile.size() > 1 && appData.activeFile < appData.pathsToFile.size() - 1) {
        ++appData.activeFile;

        UpdateActiveImage(appData);
    }
}

}  // namespace ActiveFile
