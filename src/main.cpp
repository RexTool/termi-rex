
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include "dom/editor_view.hpp"


int main(){
  bool insertMode = false;
  std::optional<std::string> commandText;

  auto screen = ftxui::ScreenInteractive::TerminalOutput();

  std::string note = "My first note";
  auto input = ftxui::Input(&note, "Type your note...");

  auto editor = ftxui::CatchEvent(input, [&insertMode, &commandText](ftxui::Event event) {
    
    if(commandText.has_value()) {
      if(event == ftxui::Event::Escape) {
        commandText.reset();
      }else if( event.is_character()) {
        commandText.value() += event.character();
      }
      return event != ftxui::Event::CtrlC;
    }
    
    if(!insertMode && event == ftxui::Event::Character(':')) {
        commandText = "";
        return true;
    }

    if(event == ftxui::Event::Escape) {
        insertMode = false;
        return true;
    }
    
    if(!insertMode && event == ftxui::Event::i) {
        insertMode = true;
        return true;
    }

    if(event == ftxui::Event::CtrlC) {
      return false;
    }
    return !insertMode;
  });

  auto view = ftxui::Renderer(editor, [&] {
      return renderEditorView(input->Render(), insertMode, "Untitled", commandText);
  });
  

  screen.Loop(view);
  return 0;
}