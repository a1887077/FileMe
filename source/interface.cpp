#include <fileme/interface.h>

#include <algorithm>
#include <cctype>
#include <string>

Interface::Interface() {
  // get the number of rows and columns
  getmaxyx(stdscr, this->term_row, this->term_col);

  // update the number of entries in the current directory
  this->list_entries = this->numEntries();
  this->scroll_depth = 0;

  // height, width, starty, startx
  this->file_list_win = newwin(this->term_row - 5, this->term_col, 1, 0);
  this->command_entry_win = newwin(1, this->term_col, this->term_row - 1, 0);
}

Interface::~Interface() {
  if (this->file_list_win != nullptr) {
    delwin(this->file_list_win);
  }
  if (this->command_entry_win != nullptr) {
    delwin(this->command_entry_win);
  }
}

/**
 * @brief Ask the user for confirmation of a command
 * @retval A boolean representing the user's response (true = confirmed, false = not confirmed)
 */
bool Interface::ask_confirmation(void) {
  wclear(this->command_entry_win);

  char response[3];

  wprintw(this->command_entry_win, "Do you confirm? [Y/N]: ");
  wrefresh(this->command_entry_win);
  wgetstr(this->command_entry_win, response);

  if (response == "Y" || response == "y") {
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

  wprintw(this->command_entry_win, "Enter a file name (max 20 char): ");
  wrefresh(this->command_entry_win);
  wgetstr(this->command_entry_win, response);

  return std::string(response);
}

/**
 * @brief Show a message to the user
 * @param message The message to display
 */
void Interface::show_message(std::string message) {
  wclear(this->command_entry_win);

  wprintw(this->command_entry_win, message.c_str());
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
  wattron(this->command_entry_win, COLOR_RED);
  wprintw(this->command_entry_win, error.c_str());
  wattroff(this->command_entry_win, COLOR_RED);
  wrefresh(this->command_entry_win);
}