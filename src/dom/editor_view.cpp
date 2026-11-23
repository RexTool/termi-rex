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