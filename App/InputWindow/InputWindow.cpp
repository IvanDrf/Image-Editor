#include "InputWindow.hpp"

#include <limits>
#include <vector>

namespace {
const short kMaxPathLength{std::numeric_limits<short>::max() - 1};
}

namespace Window {
std::string AddFileWindow() {
    std::string command = "zenity --file-selection --title=\"Select file\" 2>/dev/null";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "";
    }

    char buffer[kMaxPathLength];
    std::string result{};
    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }

    int status = pclose(pipe);
    if (status != 0) {
        return "";
    }

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

std::string SaveFileWindow(const std::string& fileName) {
    std::string command = "zenity --file-selection --save --title=\"Save File\" 2>/dev/null";

    if (!fileName.empty()) {
        command += " --filename=\"" + fileName + "\"";
    }

    command += " 2>/dev/null";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "";
    }

    char buffer[kMaxPathLength];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    int status = pclose(pipe);
    if (status != 0) {
        return "";
    }

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}
}  // namespace Window

// Main buttons functions
namespace Front {
using Paths = const std::vector<std::string>;

std::string AddFile([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return Window::AddFileWindow();
}

std::string DeleteFile(Paths& pathsToFile, size_t activeFile) {
    if (activeFile < pathsToFile.size()) {
        return pathsToFile[activeFile];
    }

    return "";
}

std::string SaveFile(Paths& pathsToFile, size_t activeFile) {
    if (activeFile < pathsToFile.size()) {
        return Window::SaveFileWindow(pathsToFile[activeFile]);
    }

    return "";
}

std::string SelectBrush([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return "";
}

std::string MoveImage([[maybe_unused]] Paths& pathsToFile, [[maybe_unused]] size_t activeFile) {
    return "";
}
}  // namespace Front
