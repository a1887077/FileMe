#ifndef INTERFACE_H
#define INTERFACE_H

#include <curses.h>
#include <fileme/manager.h>

#include <string>

class Interface : public Manager {
private:
  // pointers to the curses windows used in the UI
  WINDOW *file_list_win;
  WINDOW *control_list_win;
  WINDOW *command_entry_win;

  // the number of rows and columns available in the terminal
  int term_row;
  int term_col;

  // the number of entries in the file list, and the depth of the scrolling
  int list_entries;
  int scroll_offset;
  int highlighted_item;

public:
  // class constructor / destructor
  Interface();
  ~Interface();

  // function to show the available controls
  void show_controls(bool copied);

  // functions related to the list of files
  void show_file_list(void);
  DirEntry get_highlighted(void);
  int get_file_command(void);
  void scroll_up(void);
  void scroll_down(void);

  // functions related to the command entry window
  bool ask_confirmation(void);
  std::string ask_filename(void);
  void show_path(void);
  void show_message(std::string message);
  void show_error(std::string error);

  /**
   * @brief Navigate into a different directory, moving the workspace directory
   * @param target_dir The directory to mvoe into
   * @retval 0 on success, or negative FileOperator::OperatorError code on failure
   */
  int nav_into_dir(DirEntry target_dir);

  /**
   * @brief Navigate up one directory level, updating the workspace directory
   * @retval 0 on success, or negative FileOperator::OperatorError code on failure
   */
  int nav_out_of_dir(void);
};

#endif // INTERFACE_H