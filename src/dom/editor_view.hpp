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
    ftxui::Element body, bool insertMode, std::string title = "Untitled",
    const std::optional<std::string>& commandText = std::nullopt);// Starting Render View for the editor component
ftxui::Element showTitleRow(std::string title);
ftxui::Element showStatusRow(
    bool insertMode,
    const std::optional<std::string>& commandText = std::nullopt);




#endif