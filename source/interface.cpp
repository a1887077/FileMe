#include <fileme/interface.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>

/**
 * @brief A constructor for the interface class
 */
Interface::Interface() {
  // initialise the curses screen
  initscr();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  start_color();
  use_default_colors();

  // colour definitions
  init_pair(1, COLOR_GREEN, -1);
  init_pair(2, COLOR_RED, -1);

  // get the number of rows and columns available
  getmaxyx(stdscr, this->term_row, this->term_col);

  // update the number of entries in the current directory
  this->list_entries = this->term_row - 7;
  this->scroll_offset = 0;
  this->highlighted_item = 0;

  // define windows of the different interface areas (height, width, starty, startx)
  this->file_list_win = newwin(this->term_row - 7, this->term_col - 2, 1, 1);
  this->control_list_win = newwin(4, this->term_col - 2, this->term_row - 6, 1);
  this->command_entry_win = newwin(1, this->term_col - 2, this->term_row - 2, 1);

  // render the controls menu
  this->show_controls(false);

  // configure the file list window and render it's initial state
  keypad(this->file_list_win, TRUE);
  this->show_file_list();
}

/**
 * @brief Interface class deconstructor
 */
Interface::~Interface() {
  // delete each of the windows if able to
  if (this->file_list_win != nullptr) {
    delwin(this->file_list_win);
  }
  if (this->control_list_win != nullptr) {
    delwin(this->control_list_win);
  }
  if (this->command_entry_win != nullptr) {
    delwin(this->command_entry_win);
  }

  // delete the main window and stop cursess
  endwin();
}

/** 
 * @brief User feedback function which returns the 
 * feedback to the user based on the command
 */
void Interface::show_user_feedback(int return_value){
  switch (return_value) {
    case FileOperator::SUCCESS:
      this->show_message("command completed successfully"); // if renamed successfully, show success message
      this->show_file_list(); // update the display interface
      this->show_path(); // show current path
      break;
    case -FileOperator::E_ITEM_DOES_NOT_EXIST:
      this->show_error("selected operation cannot be performed on the current file/directory"); // if file/directory does not exist
      break;
    case -FileOperator::E_ITEM_EXISTS:
      this->show_error("file/directory already exists"); // if new file name already exists in the database
      break;
    case -FileOperator::E_DIFFERENT_TYPES:
      this->show_error("invalid conversion requested"); // heuristic check
      break;
    case -FileOperator::E_INVALID_PATH:
      this->show_error("file/directory does not exist");
      break;
    case -FileOperator::E_INVALID_TYPE:
      this->show_error("selected object is not a directory");
      break;
    default:
      this->show_error("unknown error occured"); // default error message
      break;
  }
}

/**
 * @brief Render the list of controls to the control list window
 * @param copied A flag to indicate if there is a saved file which has been copied
 */
void Interface::show_controls(bool copied) {
  // clear the control list window
  wclear(this->control_list_win);

  // print the upper boarder
  for (int i = 0; i < this->term_col - 2; i++) {
    wprintw(this->control_list_win, "=");
  }

  // print initial controls
  wprintw(this->control_list_win,
          " [N] - New File   [R] - Rename File           [D] - Delete File            ");

  // highlight the [C] label if an item has been copied
  if (copied) {
    wattron(this->control_list_win, A_REVERSE);
  }
  wprintw(this->control_list_win, "[C]");  // print the [C] label
  if (copied) {
    wattroff(this->control_list_win, A_REVERSE);  // disable highlighting
  }

  // print the remaining controls
  wprintw(this->control_list_win,
          " - Copy \n [P] - Paste      [ENTER] - Select Directory  [BKSP] - Previous Directory  [Q] - Quit\n");

  // print the bottom border
  for (int i = 0; i < this->term_col - 2; i++) {
    wprintw(this->control_list_win, "=");
  }

  // display the control list
  wrefresh(this->control_list_win);
}

/**
 * @brief render the list of files in the current directory
 */
void Interface::show_file_list(void) {
  // clear the file list contents
  wclear(this->file_list_win);

  // get the current list of files of the current working directory
  std::vector<DirEntry> entries = this->getEntries();

  // loop through each available slot in the file list
  for (int i = 0; i < this->list_entries; i++) {
    // calculate the index into the list of files
    int index = i + this->scroll_offset;

    // if the current index is selected by the user, highlight it
    if (index == this->highlighted_item) {
      wattron(this->file_list_win, COLOR_PAIR(1) | A_REVERSE);
    }

    if (i == 0 && scroll_offset == 0) {
      // we are at the top of the list, display the exit directory symbol
      wprintw(this->file_list_win, "   [..]\n");
    } else if (i == 0 && scroll_offset > 0) {
      // we have scrolled down, display a 'more contents' symbol
      wprintw(this->file_list_win, "   ...\n");
    } else if (index - 1 < (int)this->numEntries() && i != this->list_entries - 1) {
      // display a directory entry
      DirEntry entry = entries[index - 1];
      char dirSeparator
          = entry.getType() == DIRECTORY_ENTRY ? '/' : ' ';  // if the directory is a file or not
      wprintw(this->file_list_win, "   %s%c\n", entry.getName().c_str(), dirSeparator);
    } else if (index - 1 < (int)this->numEntries() && i == this->list_entries - 1) {
      // we have scrolled up, display a 'more contents' symbol
      wprintw(this->file_list_win, "   ...\n");
    }

    // turn off highlighting
    if (index == this->highlighted_item) {
      wattroff(this->file_list_win, COLOR_PAIR(1) | A_REVERSE);
    }
  }

  // display the file list
  wrefresh(this->file_list_win);
}

/**
 * @brief Get a command from the point of reference of the file list window
 * @retval An integer representing the character command which was read (from getchr)
 */
int Interface::get_file_command(void) { return wgetch(this->file_list_win); }

/**
 * @brief Get the currently highlighted filentry
 * @retval A DirEntry object of the entry which is currently highlighted
 */
int Interface::get_highlighted(DirEntry &highlighted) {
  if (this->highlighted_item == 0) {
    return -1;
  }

  highlighted = this->getEntries().at(this->highlighted_item - 1);

  return 0;
}

/**
 * @brief Move the current file selection up in the file list
 */
void Interface::scroll_up(void) {
  // is it possible to scroll up
  if (this->highlighted_item > 0) {
    this->highlighted_item--;
  }

  // will the cursor scroll outside of visible bounds
  if (this->highlighted_item <= this->scroll_offset && this->scroll_offset > 0) {
    this->scroll_offset--;
  }
}

/**
 * @brief Move the current file selection down in the file list
 */
void Interface::scroll_down(void) {
  // is it possible to scroll down
  if (this->highlighted_item < (int)this->numEntries()) {
    this->highlighted_item++;
  }

  // will the cursor scroll outside of visible bounds
  if (this->highlighted_item >= this->list_entries + this->scroll_offset - 1) {
    this->scroll_offset++;
  }
}

/**
 * @brief Ask the user for confirmation of a command
 * @retval A boolean representing the user's response (true = confirmed, false = not confirmed)
 */
bool Interface::ask_confirmation(void) {
  // clear the command entry window
  wclear(this->command_entry_win);

  // c-style string to store the response from the user as required by curses
  char response[3];

  echo();                                                          // we want to see the input
  wprintw(this->command_entry_win, "   Do you confirm? [Y/N]: ");  // prompt the user
  wrefresh(this->command_entry_win);                               // display the prompt
  wgetstr(this->command_entry_win, response);                      // wait for input
  noecho();                                                        // input received, turn off echo

  // convert the c-style string into a c++ string
  std::string response_string = std::string(response);

  // output based on the user's input
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
  // clear the command entry window
  wclear(this->command_entry_win);

  // c-style string to store the response from the user as required by curses
  char response[20];

  echo();  // enable the user to be able to see their input
  wprintw(this->command_entry_win, "   Enter a file name (max 20 char): ");  // prompt the user
  wrefresh(this->command_entry_win);                                         // display the prompt
  wgetstr(this->command_entry_win, response);  // wait for a response from the user
  noecho();                                    // input received, turn off echo

  // convert the c-style string to a c++ string and return it
  return std::string(response);
}

void Interface::show_path(void) { this->show_message(this->workspace_path); }

/**
 * @brief Show a message to the user
 * @param message The message to display
 */
void Interface::show_message(std::string message) {
  // clear the command entry window
  wclear(this->command_entry_win);

  // set the desired message
  wprintw(this->command_entry_win, "   %s", message.c_str());

  // display the desired message
  wrefresh(this->command_entry_win);
}

/**
 * @brief Show an error message to the user
 * @param error The error message to display
 */
void Interface::show_error(std::string error) {
  // clear the command entry window
  wclear(this->command_entry_win);

  // ensure the error is upper case
  std::transform(error.begin(), error.end(), error.begin(), ::toupper);

  // print the error in red colour
  wattron(this->command_entry_win, COLOR_PAIR(2));
  wprintw(this->command_entry_win, "   %s", error.c_str());
  wattroff(this->command_entry_win, COLOR_PAIR(2));

  // display the desired error message
  wrefresh(this->command_entry_win);
}

/**
 * @brief Navigate into a different directory, moving the workspace directory
 * @param target_dir The directory to mvoe into
 * @retval 0 on success, or negative FileOperator::OperatorError code on failure
 */
int Interface::nav_into_dir(DirEntry target_dir) {
  if (target_dir.getType() != DIRECTORY_ENTRY) {
    return -E_INVALID_TYPE;
  }

  if (fs::exists(target_dir.getPath()) == false) {
    return -E_INVALID_PATH;
  }

  // update the workspace path
  this->workspace_path = target_dir.getPath();
  this->scroll_offset = 0;
  this->highlighted_item = 0;

  // update the directory entry list
  this->buildList();
  this->sortList();

  return 0;
}

/**
 * @brief Navigate up one directory level, updating the workspace directory
 * @retval 0 on success, or negative FileOperator::OperatorError code on failure
 */
int Interface::nav_out_of_dir(void) {
  this->workspace_path = this->workspace_path.parent_path();
  this->scroll_offset = 0;
  this->highlighted_item = 0;

  this->buildList();
  this->sortList();

  return 0;
}
