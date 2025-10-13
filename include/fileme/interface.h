#include <curses.h>
#include <fileme/manager.h>

#include <string>

class Interface : public Manager {
private:
  // pointers to the curses windows used in the UI
  WINDOW *file_list_win;
  WINDOW *command_entry_win;

  // the number of rows and columns available in the terminal
  int term_row;
  int term_col;

  // the number of entries in the file list, and the depth of the scrolling
  int list_entries;
  int scroll_depth;

public:
  // class constructor / destructor
  Interface();
  ~Interface();

  // functions related to the command entry window
  bool ask_confirmation(void);
  std::string ask_filename(void);
  void show_message(std::string message);
  void show_error(std::string error);
};
