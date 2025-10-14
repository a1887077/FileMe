#include <curses.h>
#include <fileme/interface.h>

#include <chrono>
#include <string>
#include <thread>

int main(int argc, char** argv) {
  Interface interface;

  interface.show_message("Welcome to FileMe.");

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  interface.show_message(" >>> ");

  bool quit = false;

  while (quit == false) {
    int command = interface.get_file_command();

    switch (command) {
      case 'q':
      case KEY_EXIT:
        interface.show_message("Bye...");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        quit = true;
        break;

      case KEY_UP:
        interface.show_message("scroll up");
        interface.scroll_up();
        interface.show_file_list();
        break;

      case KEY_DOWN:
        interface.show_message("scroll down");
        interface.scroll_down();
        interface.show_file_list();
        break;

      case KEY_ENTER:
        interface.show_message("enter directory");
        break;

      case KEY_BACKSPACE:
        interface.show_message("leave directory");
        break;

      case 'n':
      case 'N':
        interface.show_message("new file");
        interface.ask_filename();

        break;

      case 'r':
      case 'R':
        interface.show_message("rename file");
        interface.ask_filename();
        break;

      case 'd':
      case 'D':
        interface.show_message("delete file");
        interface.ask_confirmation();
        break;

      case 'c':
      case 'C':
        interface.show_message("copy file");
        break;

      case 'p':
      case 'P':
        interface.show_message("paste file");
        break;

      default:
        interface.show_error("invalid command");
        break;
    }
  }

  return 0;
}