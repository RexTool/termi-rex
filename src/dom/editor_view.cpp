/*
 * Owner: RexTool
 * Description: Editor view component for RexTool. Handles the display and interaction of the text editor within the UI.
 */ 
#include "editor_view.hpp"
#include <iostream>

#include <ftxui/dom/elements.hpp>

// Creates a Line
ftxui::Element createLine(const std::string& content) {
    return ftxui::text(content);
}

ftxui::Element showStatusRow(
    bool insertMode,
    const std::optional<std::string>& commandText,
    const std::string& statusMessage) {
    if (commandText.has_value()) {
        return createLine(":" + commandText.value());
    }

    std::string row = insertMode ? "INSERT" : "NORMAL";
    if (!statusMessage.empty()) {
        row += " - " + statusMessage;
    }
    return createLine(row);
}

ftxui::Element showTitleRow(std::string title, bool modified) {
    if (title.empty()) {
        title = "Untitled";
    }
    if (modified) {
        title += " [+]";
    }
    return createLine("Termi-Rex - " + title);
}

ftxui::Element renderEditorView(
    ftxui::Element body,
    bool insertMode,
    std::string title,
    const std::optional<std::string>& commandText,
    bool modified,
    const std::string& statusMessage) {
    return ftxui::vbox({
        showTitleRow(title, modified),
        body | ftxui::border | ftxui::flex,
        showStatusRow(insertMode, commandText, statusMessage)
    });
}