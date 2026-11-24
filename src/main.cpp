#include <iostream>
#include <optional>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "document/document.hpp"
#include "dom/editor_view.hpp"


int main(int argc, char* argv[]){
  if(argc > 2) {
    std::cerr << "Usage: App [file]\n";
    return 1;
  }

  const std::string filename = argc == 2 ? argv[1] : "note.txt";
  NoteFile noteFile(filename);

  std::string note;
  if(!noteFile.load(note)) {
    std::cerr << "Could not open " << filename << " as a text file\n";
    return 1;
  }

  std::string originalNote = note;
  std::string statusMessage;
  bool insertMode = false;
  std::optional<std::string> commandText;

  auto screen = ftxui::ScreenInteractive::TerminalOutput();
  auto input = ftxui::Input(&note, "type....");

  auto editor = ftxui::CatchEvent(input, [&](ftxui::Event event) {
    if(commandText.has_value()) {
      if(event == ftxui::Event::Escape) {
        commandText.reset();

      } else if(event == ftxui::Event::Return){
        if(commandText.value() == "w" || commandText.value() == "wq") {
          if(noteFile.save(note)) {
            originalNote = note;
            statusMessage = "saved";

            if(commandText.value() == "wq") {
              screen.Exit();
            }
          } else {
            statusMessage = "saved failed";
          }
        } else if (commandText.value() == "q") {
          if (note == originalNote) {
            screen.Exit();
          } else {
            statusMessage = "unsaved changes - use :wq to save or :q! to discard";
          }
        } else if (commandText.value() == "q!") {
          statusMessage = "quite without saving";
          screen.Exit();
        } else {
          statusMessage = "unknown command";
        }
        commandText.reset();
      } else if( event.is_character()) {
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
        statusMessage.clear();
        return true;
    }

    if(event == ftxui::Event::CtrlC || insertMode) return false;
    if (event == ftxui::Event::h) {
      input->OnEvent(ftxui::Event::ArrowLeft);
    } else if (event == ftxui::Event::j) {
      input->OnEvent(ftxui::Event::ArrowDown);
    } else if (event == ftxui::Event::k) {
      input->OnEvent(ftxui::Event::ArrowUp);
    } else if (event == ftxui::Event::l) {
      input->OnEvent(ftxui::Event::ArrowRight);
    }
    return true;
  });

  auto view = ftxui::Renderer(editor, [&] {
    return renderEditorView(
        input->Render(),
        insertMode,
        filename,
        commandText,
        note != originalNote,
        statusMessage
    );
  });

  screen.Loop(view);
  return 0;
}