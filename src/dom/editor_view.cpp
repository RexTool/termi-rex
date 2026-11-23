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

ftxui::Element showStatusRow(bool insertMode, const std::optional<std::string>& commandText) {
    if (commandText.has_value()) return createLine(":" + commandText.value());
    if (insertMode) return createLine("INSERT");
    return createLine("NORMAL");
}
ftxui::Element showTitleRow(std::string title) {
    return createLine("Termi-Rex - " + title);
}

ftxui::Element renderEditorView(
    ftxui::Element body, 
    bool insertMode,
    std::string title,
    const std::optional<std::string>& commandText) {
    
        return ftxui::vbox({
            showTitleRow(title),
            body | ftxui::border | ftxui::flex,
            showStatusRow(insertMode, commandText)
    });
}