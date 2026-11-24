/*
 * Owner: RexTool
 * Description: Editor view component for RexTool. Handles the display and interaction of the text editor within the UI.
 */ 

 #ifndef EDITOR_VIEW_HPP
#define EDITOR_VIEW_HPP

#include <string>
#include <optional>

#include <ftxui/dom/elements.hpp>

ftxui::Element createLine(const std::string& content);
ftxui::Element renderEditorView(
    ftxui::Element body,
    bool insertMode,
    std::string title = "Untitled",
    const std::optional<std::string>& commandText = std::nullopt,
    bool modified = false,
    const std::string& statusMessage = "");


ftxui::Element showTitleRow(std::string title, bool modified = false);
ftxui::Element showStatusRow(
    bool insertMode,
    const std::optional<std::string>& commandText = std::nullopt,
    const std::string& statusMessage = "");




#endif