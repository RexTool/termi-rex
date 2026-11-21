#include <iostream>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>

int main() {
  using namespace ftxui;
  
  auto screen = Screen::Create(
    ftxui::Dimension::Full(),
    ftxui::Dimension::Fixed(25)
  );

  Element document = vbox({
    vbox({
        text("  > Termi-Rex") | bold | border,
    }),
    hbox({
      text("Folder Structure") | border,
      vbox({
        text("Main Content Area") | flex,
      }) | border | flex,
    }) | flex,
  });
 
  Render(screen, document);

  screen.Print();
  return 0;
}