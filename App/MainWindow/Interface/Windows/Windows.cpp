#include "../Interface.hpp"

namespace {
const short kMaxPathLength{std::numeric_limits<short>::max() - 1};
}

namespace Interface {
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
}  // namespace Interface
