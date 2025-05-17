#include "Interface.hpp"

#include <limits>

#include "../../FileField/FileField.hpp"
#include "../MainWindow.hpp"

namespace Interface {

std::string AddFile([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return AddFileWindow();
}

std::string DeleteFile(Paths& pathsToFile, size_t activeFile) {
    if (activeFile < pathsToFile.size()) {
        return pathsToFile[activeFile];
    }

    return "";
}

std::string SaveFile(Paths& pathsToFile, size_t activeFile) {
    if (activeFile < pathsToFile.size()) {
        return SaveFileWindow(pathsToFile[activeFile]);
    }

    return "";
}

std::string SelectBrush([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return "";
}

std::string MoveImage([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return "";
}

std::string Reset([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return "";
}
}  // namespace Interface
