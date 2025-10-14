#include <fileme/interface.h>

#include <algorithm>
#include <cctype>
#include <string>

Interface::Interface() {
  initscr();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  start_color();
  use_default_colors();

  init_pair(1, COLOR_GREEN, -1);
  init_pair(2, COLOR_RED, -1);

  // get the number of rows and columns
  getmaxyx(stdscr, this->term_row, this->term_col);

  // update the number of entries in the current directory
  this->list_entries = this->term_row - 7;
  this->scroll_offset = 0;
  this->highlighted_item = 0;

  // height, width, starty, startx
  this->file_list_win = newwin(this->term_row - 7, this->term_col - 2, 1, 1);
  this->control_list_win = newwin(4, this->term_col - 2, this->term_row - 6, 1);
  this->command_entry_win = newwin(1, this->term_col - 2, this->term_row - 2, 1);

  this->show_controls(false);

  keypad(this->file_list_win, TRUE);
  this->show_file_list();

  /* box(this->file_list_win, 0, 0);
  wrefresh(this->file_list_win); */
}

Interface::~Interface() {
  if (this->file_list_win != nullptr) {
    delwin(this->file_list_win);
  }
  if (this->command_entry_win != nullptr) {
    delwin(this->command_entry_win);
  }

  endwin();
}

void Interface::show_controls(bool copied) {
  wclear(this->control_list_win);

  for (int i = 0; i < this->term_col - 2; i++) {
    wprintw(this->control_list_win, "=");
  }

  wprintw(this->control_list_win,
          " [N] - New File   [R] - Rename File           [D] - Delete File            ");
  if (copied) {
    wattron(this->control_list_win, A_REVERSE);
  }
  wprintw(this->control_list_win, "[C]");
  if (copied) {
    wattroff(this->control_list_win, A_REVERSE);
  }
  wprintw(this->control_list_win,
          " - Copy \n [P] - Paste      [ENTER] - Select Directory  [BKSP] - Previous Directory \n");

  for (int i = 0; i < this->term_col - 2; i++) {
    wprintw(this->control_list_win, "=");
  }

  wrefresh(this->control_list_win);
}

/**
 * @brief render the list of files in the current directory
 */
void Interface::show_file_list(void) {
  wclear(this->file_list_win);

  std::vector<DirEntry> entries = this->getEntries();

  for (int i = 0; i < this->list_entries; i++) {
    int index = i + this->scroll_offset;

    if (index == this->highlighted_item) {
      wattron(this->file_list_win, COLOR_PAIR(1) | A_REVERSE);
    }

    if (i == 0 && scroll_offset == 0) {
      wprintw(this->file_list_win, "   [..]\n");
    } else if (i == 0 && scroll_offset > 0) {
      wprintw(this->file_list_win, "   ...\n");
    } else if (index - 1 < entries.size() && i != this->list_entries - 1) {
      DirEntry entry = entries[index - 1];
      char dirSeparator = entry.getType() == DIRECTORY_ENTRY ? '/' : ' ';

      wprintw(this->file_list_win, "   %s%c\n", entry.getName().c_str(), dirSeparator);
    } else if (index - 1 < entries.size() && i == this->list_entries - 1) {
      wprintw(this->file_list_win, "   ...\n");
    }

    if (index == this->highlighted_item) {
      wattroff(this->file_list_win, COLOR_PAIR(1) | A_REVERSE);
    }
  }

  wrefresh(this->file_list_win);
}

int Interface::get_file_command(void) { return wgetch(this->file_list_win); }

void Interface::scroll_up(void) {
  if (this->highlighted_item > 0) {
    this->highlighted_item--;
  }

  if (this->highlighted_item < this->scroll_offset) {
    this->scroll_offset--;
  }
}

void Interface::scroll_down(void) {
  if (this->highlighted_item < this->numEntries()) {
    this->highlighted_item++;
  }

  if (this->highlighted_item > this->list_entries + this->scroll_offset - 1) {
    this->scroll_offset++;
  }
}

/**
 * @brief Ask the user for confirmation of a command
 * @retval A boolean representing the user's response (true = confirmed, false = not confirmed)
 */
bool Interface::ask_confirmation(void) {
  wclear(this->command_entry_win);

  char response[3];

  echo();
  wprintw(this->command_entry_win, "Do you confirm? [Y/N]: ");
  wrefresh(this->command_entry_win);
  wgetstr(this->command_entry_win, response);
  noecho();

  std::string response_string = std::string(response);

  if (response_string == "Y" || response_string == "y") {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Ask the user for a filename
 * @retval A string representing the entered filename
 */
std::string Interface::ask_filename(void) {
  wclear(this->command_entry_win);

  char response[20];

  echo();
  wprintw(this->command_entry_win, "   Enter a file name (max 20 char): ");
  wrefresh(this->command_entry_win);
  wgetstr(this->command_entry_win, response);
  noecho();

  return std::string(response);
}

/**
 * @brief Show a message to the user
 * @param message The message to display
 */
void Interface::show_message(std::string message) {
  wclear(this->command_entry_win);

  wprintw(this->command_entry_win, "   %s", message.c_str());

  wrefresh(this->command_entry_win);
}

/**
 * @brief Show an error message to the user
 * @param error The error message to display
 */
void Interface::show_error(std::string error) {
  wclear(this->command_entry_win);

  // ensure the error is upper case
  std::transform(error.begin(), error.end(), error.begin(), ::toupper);

  // print the error in red colour
  wattron(this->command_entry_win, COLOR_PAIR(2));
  wprintw(this->command_entry_win, "   %s", error.c_str());
  wattroff(this->command_entry_win, COLOR_PAIR(2));

  wrefresh(this->command_entry_win);
}