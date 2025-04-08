#include "InputWindow.hpp"

#include <limits>

namespace {
const short kMaxPathLength{std::numeric_limits<short>::max() - 1};
}

namespace Window {
std::string AddFileWindow() {
    std::string command = "zenity --file-selection --title=\"Select file\" 2>/dev/null";

    // Открываем pipe для чтения вывода команды
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "";
    }

    // Читаем результат (путь к файлу)
    char buffer[kMaxPathLength];
    std::string result{};
    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }

    // Закрываем pipe и проверяем статус завершения
    int status = pclose(pipe);
    if (status != 0) {
        return "";  // Пользователь закрыл окно или произошла ошибка
    }

    // Убираем символ новой строки, если он есть
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

std::string SaveFileWindow(const std::string& fileName) {
    std::string command = "zenity --file-selection --save --title=\"Сохранить файл\"";

    // Если указано имя по умолчанию, добавляем его
    if (!fileName.empty()) {
        command += " --filename=\"" + fileName + "\"";
    }

    // Перенаправляем stderr в /dev/null, чтобы избежать лишних сообщений
    command += " 2>/dev/null";

    // Открываем pipe для чтения вывода команды
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "";
    }

    // Читаем результат (путь к файлу)
    char buffer[kMaxPathLength];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    // Закрываем pipe и проверяем статус завершения
    int status = pclose(pipe);
    if (status != 0) {
        return "";  // Пользователь закрыл окно или произошла ошибка
    }

    // Убираем символ новой строки, если он есть
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}
}  // namespace Window
