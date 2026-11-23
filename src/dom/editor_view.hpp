/*
 * Owner: RexTool
 * Description: Editor view component for RexTool. Handles the display and interaction of the text editor within the UI.
 */ 

#ifndef EDITOR_VIEW_HPP
#define EDITOR_VIEW_HPP

#include <string>
#include <ftxui/dom/elements.hpp>

ftxui::Element createLine(const std::string& content);

#endif