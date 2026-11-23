
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include "dom/editor_view.hpp"


int main(){
  auto screen = ftxui::ScreenInteractive::TerminalOutput();

  auto layout = ftxui::vbox({
    createLine("My first note"),
    ftxui::separator(),
    createLine("My first note"),
  });

  auto view = ftxui::Renderer([&layout] {
      return layout;
  });

  screen.Loop(view);
  return 0;
}