#include "MainMethods.hpp"

#include <gtk/gtk.h>

#include "../MainWindow/MainWindow.hpp"

// Useful functions for working with the file name and the path to the file
namespace {
std::string FindPath(std::vector<std::string>& pathToFile, const std::string& fileName) {
    for (const auto& file : pathToFile) {
        if (GetFileName(file) == fileName) {
            return file;
        }
    }

    return "";
}

void DeletePath(std::vector<std::string>& pathToFile, const std::string& fileName) {
    for (const auto& file : pathToFile) {
        if (GetFileName(file) == fileName) {
            pathToFile.erase(std::remove(pathToFile.begin(), pathToFile.end(), file), pathToFile.end());
        }
    }
}

}  // namespace

namespace Front {
std::string AddFile() {
    // Инициализируем GTK только один раз
    static bool gtk_initialized = false;
    if (!gtk_initialized) {
        gtk_init(nullptr, nullptr);
        gtk_initialized = true;
    }

    GtkWidget* dialog = gtk_file_chooser_dialog_new("Open File", nullptr, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, nullptr);

    // Устанавливаем модальность и возможность закрытия
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    std::string filename = "";
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        gchar* temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (temp) {
            filename = temp;
            g_free(temp);
        }
    }

    // Корректное уничтожение диалога
    gtk_widget_hide(dialog);
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
    gtk_widget_destroy(dialog);

    return filename;
}

std::string SaveFile() {
}

std::string DeleteFile() {
}

std::string SelectBrush() {
}
}  // namespace Front

namespace Back {
void AddFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus) {
    if (result.empty()) {
        return;
    }

    try {
        if (std::find(fileField.GetFiles().begin(), fileField.GetFiles().end(), GetFileName(result)) != fileField.GetFiles().end()) {
            statusBar.UpdateStatus("File has already been added");

            return;
        }

        sf::Image temp;
        if (!temp.loadFromFile(result)) {
            throw std::invalid_argument("Image could not be found");
        }

        pathToFile.push_back(result);      // Add new path to the new file
        image.ClearImage(previousStatus);  // Clear if there was some image
        image.LoadImage(result);           // Load new image
        image.SetMainImageScale();

        fileField.AddFile(GetFileName(result));

        statusBar.UpdateStatus("Image loaded successfully", sf::Color::Green);
    } catch (std::exception& e) {
        statusBar.UpdateStatus("Image could not be found", sf::Color::Red);
    }
}

void SaveFile(const std::string& result, Image& image, StatusBar& statusBar) {
    if (!result.empty() && image.SaveImage(result)) {
        statusBar.UpdateStatus("File was saved successfully", sf::Color::Green);

        return;
    }

    statusBar.UpdateStatus("Unable to save the file", sf::Color::Red);
}

void DeleteFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus) {
    if (result.empty()) {
        return;
    }

    size_t oldLength{fileField.GetFiles().size()};  // Old count of files
    fileField.DeleteFile(GetFileName(result));
    size_t newLength{fileField.GetFiles().size()};  // New count of files

    if (oldLength == newLength) {
        statusBar.UpdateStatus("File not found", sf::Color::Red);

        return;
    }

    statusBar.UpdateStatus("File was deleted successfully", sf::Color::Green);

    image.ClearImage(previousStatus);
    DeletePath(pathToFile, result);  // Delete path to deleting file
}

void SelectFile(std::vector<std::string>& pathToFile, const std::string& result, Image& image, FileField& fileField, StatusBar& statusBar, std::stack<sf::Image>& previousStatus) {
    if (!result.empty() && std::find(fileField.GetFiles().begin(), fileField.GetFiles().end(), result) != fileField.GetFiles().end()) {
        statusBar.UpdateStatus("File " + result + " selected", sf::Color::Green);

        image.ClearImage(previousStatus);               // Clear old image
        image.LoadImage(FindPath(pathToFile, result));  // Find images path and select image
        image.SetMainImageScale();

        return;
    }

    statusBar.UpdateStatus("File " + result + " cannot be selected", sf::Color::Red);
}

void SelectBrush(bool& brushPressed, const Image& image, StatusBar& statusBar) {
    brushPressed = !brushPressed && image.HasImage();

    if (brushPressed) {
        statusBar.UpdateStatus("Brush selected", sf::Color::Green);
        return;
    }

    statusBar.UpdateStatus("Brush is no longer selected");
}

}  // namespace Back
