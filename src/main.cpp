#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

namespace fs = std::filesystem;

namespace {
  bool load_file(const fs::path& path, std::string& contents, std::vector<bool>& crlf) {
    std::error_code error;
    const auto status = fs::symlink_status(path, error);

    if (status.type() == fs::file_type::not_found) return true;
    if (!fs::is_regular_file(path, error)) return false;
    
    std::ifstream file(path, std::ios::binary);
    std::string line;

    while (std::getline(file, line)) {
      if (line.contains('\0')) return false;
      if (!file.eof()) {
        crlf.push_back(line.ends_with('\r'));
        if (crlf.back()) line.pop_back();
        line += '\n';
      }
      contents += line;
    }

    return file.eof() && !file.bad();
  }

  bool save_file(const fs::path& path, const std::string& contents, const std::vector<bool>& crlf) {
    std::error_code error;
    const auto entry = fs::symlink_status(path, error);
    const auto target = fs::is_symlink(entry) ? fs::canonical(path, error) : path;
    if (error && entry.type() != fs::file_type::not_found) return false;

    const auto status = fs::status(target, error);
    if (error && error != std::errc::no_such_file_or_directory) return false;
    if (fs::exists(status) &&
        (!fs::is_regular_file(status) || !std::fstream(target, std::ios::in | std::ios::out))) {
      return false;
    }

    // Finish writing beside the target before replacing the file.
    const auto temporary = target.parent_path() / (".termi-rex-" + std::to_string(std::random_device{}()) + ".tmp");
    std::ofstream file(temporary, std::ios::binary | std::ios::noreplace);
    if (!file) return false;
    error.clear();
    if (fs::exists(status)) fs::permissions(temporary, status.permissions(), error);
    if (!error) {
      std::size_t line = 0;
      for (char c : contents) {
        if (c == '\n') {
          const bool use_crlf = line < crlf.size() ? crlf[line] : !crlf.empty() && crlf.back();
          if (use_crlf) file.put('\r');
          ++line;
        }
        file.put(c);
      }
    }
    file.close();
    if (file && !error) {
      fs::rename(temporary, target, error);
      if (!error) return true;
    }
    fs::remove(temporary, error);
    return false;
  }

} 


int main(int argc, char* argv[]) {
  using namespace ftxui;

  if (argc > 2) {
    std::cerr << "Usage: app [file]\n";
    return 1;
  }
  const std::string filename = argc == 2 ? argv[1] : "note.txt";
  std::string saved;
  std::vector<bool> crlf;
  if (!load_file(filename, saved, crlf)) {
    std::cerr << "Could not open " << filename << " as a text file\n";
    return 1;
  }

  auto screen = App::Fullscreen();
  screen.TrackMouse(false);
  screen.ForceHandleCtrlC(false);
  std::string buffer = saved;
  std::string command, message;
  bool inserting = false;
  int cursor = 0;
  auto editor = Input(&buffer, {
    .transform = [](InputState state) { return state.element; },
    .insert = &inserting,
    .cursor_position = &cursor,
  });
  auto prompt = Input(&command, {.multiline = false});
  auto inputs = Container::Tab({editor, prompt}, nullptr);

  auto view = Renderer(inputs, [&] {
    const std::string hint = inserting ? "Esc normal" : "h/j/k/l move   i insert   :w save   :q quit";
    auto footer = prompt->Focused()
        ? hbox({text(":"), prompt->Render()})
        : hbox({text(inserting ? " INSERT " : " NORMAL ") | inverted,
                text("  " + (message.empty() ? hint : message))});
    return vbox({
      text("  > Termi-Rex   " + filename + (buffer == saved ? "" : " [+]")) | bold,
      editor->Render() | border | flex,
      footer,
    });
  });

  view |= CatchEvent([&](Event event) {
    if (event == Event::CtrlC) event = Event::Escape;
    if (prompt->Focused()) {
      if (event != Event::Return && event != Event::Escape) return false;
      editor->TakeFocus();
      if (event == Event::Escape) return true;
      if (command == "w" || command == "wq") {
        if (!save_file(filename, buffer, crlf)) {
          message = "Save failed";
          return true;
        }
        saved = buffer;
        message = "Saved";
        if (command == "wq") screen.Exit();
      } else if (command == "q!" || (command == "q" && buffer == saved)) {
        screen.Exit();
      } else {
        message = command == "q" ? "Unsaved changes. Use :wq to save or :q! to discard."
                                 : "Unknown command";
      }
      return true;
    }

    const auto can_move_left = [&] { return cursor > 0 && buffer[cursor - 1] != '\n'; };
    const auto on_character = [&] {
      return static_cast<std::size_t>(cursor) < buffer.size() && buffer[cursor] != '\n';
    };
    if (inserting) {
      if (event != Event::Escape) return event == Event::Insert;
      if (can_move_left()) editor->OnEvent(Event::ArrowLeft);
      inserting = false;
      return true;
    }

    if (event.is_character()) message.clear();
    if (event == Event::Character(':')) {
      command.clear();
      prompt->TakeFocus();
    } else if (event == Event::i || event == Event::a) {
      if (event == Event::a && on_character()) editor->OnEvent(Event::ArrowRight);
      inserting = true;
    } else if (event == Event::h && can_move_left()) {
      editor->OnEvent(Event::ArrowLeft);
    } else if (event == Event::l && on_character()) {
      editor->OnEvent(Event::ArrowRight);
    } else if (event == Event::j && buffer.find('\n', cursor) != std::string::npos) {
      editor->OnEvent(Event::ArrowDown);
    } else if (event == Event::k && cursor > 0 &&
               buffer.rfind('\n', cursor - 1) != std::string::npos) {
      editor->OnEvent(Event::ArrowUp);
    } else if (event == Event::x && on_character()) {
      editor->OnEvent(Event::Delete);
    }
    // Normal mode rests on the last character of a nonempty line.
    if (!inserting && !on_character() && can_move_left()) editor->OnEvent(Event::ArrowLeft);
    return true;
  });

  screen.Loop(view);
}
