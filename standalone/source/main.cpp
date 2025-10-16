#include <curses.h>
#include <fileme/interface.h>
#include <fileme/fileOperator.h>

#include <chrono>
#include <string>
#include <thread>

// TODO:
// 1) display doesnt update after rename, paste (rename happens, but file name doesnt change unless you go into another directory and come back)
// 2) update so it doesnt take commands at the [...] at the top (it lets you do it but obv nothing happens and error is thrown)

// a1885288: Varad Sharma, a1887077: Aidan Matthews
// main file for the fileMe program
int main(int argc, char** argv) {
  Interface interface; // create the instance of the display interface

  interface.show_message("Welcome to FileMe.");
  // wait 500 milliseconds to allow a smoother user experience
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  // remove any messages (if any)
  interface.show_message(" ");

  // declare variables used while performing commands
  bool quit = false; // when the user wants to quit
  bool userResponse = false; // to store user-based decisions
  int return_code = 0;
  DirEntry highlighted;
  std::string new_filename; // string to store data whilst user attempts different commands

  while (quit == false) {
    int command = interface.get_file_command(); // fetch's user inputs (key press or string inputs depending on the problem)
    int valid_list_entry = interface.get_highlighted(highlighted); // highlights the current file/directory for the user

    switch (command) {
      case 'q': 
      case KEY_EXIT:
        interface.show_message("Bye..."); // show the quit display message
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
        // wait 500 milliseconds for a more realistic feedback (not instant robotic quit)
        quit = true; // quit flag set to true
        break;

      case KEY_UP:
        interface.scroll_up(); // move the cursor up
        interface.show_message(highlighted.getPath()); // show message in display interface
        interface.show_file_list(); // update the display interface
        break;

      case KEY_DOWN:
        interface.scroll_down(); // move the cursor down
        interface.show_message(highlighted.getPath()); // show message in display interface
        interface.show_file_list(); // update the display interface
        break;

      case 10:
      case KEY_ENTER:
        interface.show_message("enter directory"); // show message when entering directory

        if (highlighted.getType() == DIRECTORY_ENTRY && valid_list_entry == 0) { // if the selected object is a directory
          interface.nav_into_dir(highlighted);  // enter the directory
          interface.show_file_list(); // update the display interface
          interface.show_path(); // show current path
        } else if (valid_list_entry == -1) { // if the current object is [...] (at the top)
          interface.nav_out_of_dir(); // leave the directory
          interface.show_file_list(); // update the display interface
          interface.show_path(); // show current path
        } else {
          interface.show_error("Cannot enter - is not a directory"); // error when 'ENTER' key is pressed on a file
        }

        break;

      case KEY_BACKSPACE:
        interface.show_message("leave directory"); // show display message to user
        interface.nav_out_of_dir(); // leave the current directory
        interface.show_file_list(); // update the interface display
        interface.show_path(); // show current path
        break;

      case 'n':
      case 'N':
        interface.show_message("new file");
        new_filename = interface.ask_filename(); // prompt the user for a file name
        if (interface.create(new_filename, FILE_ENTRY) == FileOperator::SUCCESS) {
          interface.show_message("file created successfully"); // show success message to user
          interface.show_file_list(); // update the display interface
        } else if(interface.create(new_filename, FILE_ENTRY) == -FileOperator::E_ITEM_EXISTS) {
          interface.show_error("file already exists"); // if file already exists in the database
        } else {
          interface.show_error("unknown error occured"); // default error message
        }

        break;

      case 'r':
      case 'R':
        interface.show_message("rename file");
        new_filename = interface.ask_filename(); // promt the user for a new file name
        if (interface.rename(highlighted, new_filename) == FileOperator::SUCCESS) {
          interface.show_message("file renamed successfully"); // if renamed successfully, show success message
          interface.show_file_list(); // update the display interface
        } else if(interface.rename(highlighted, new_filename) == -FileOperator::E_ITEM_DOES_NOT_EXIST) {
          interface.show_error("cannot rename currently selected object"); // current item cannot be renamed
        } else if(interface.rename(highlighted, new_filename) == -FileOperator::E_ITEM_EXISTS){
          interface.show_error("another file with new name exists"); // if new file name already exists in the database
        } else if(interface.rename(highlighted, new_filename) == -FileOperator::E_DIFFERENT_TYPES){
          interface.show_error("invalid conversion requested"); // heuristic check
        } else {
          interface.show_error("unknown error occured"); // default error message
        }

        break;

      case 'd':
      case 'D':
        interface.show_message("delete file");
        userResponse = interface.ask_confirmation();
        if(userResponse && interface.remove(highlighted) == FileOperator::SUCCESS){ // if the user confirms [Y] and deletion is a success
          interface.show_message("file deleted successfully"); // show success message
          interface.show_file_list(); // update the display interface
        } else if(userResponse && interface.remove(highlighted) == -FileOperator::E_ITEM_DOES_NOT_EXIST){
          interface.show_error("object to remove does not exist"); // error when object does not exist
        } else {
          interface.show_error("unknown error occured"); // default error message
        }

        break;

      case 'c':
      case 'C':
        interface.show_message("copy file");
        if(interface.copy(highlighted) == FileOperator::SUCCESS){ // if item is copied successfully
          interface.show_message("item copied successfully"); // show success message
          interface.show_file_list(); // update display interface
          interface.show_path(); // show current path
        } else {
          interface.show_error("unknown error occured"); // default error message
        }

        break;

      case 'p':
      case 'P':
        interface.show_message("paste file");
        if(interface.paste() == FileOperator::SUCCESS){ // if item is pasted correctly
          interface.show_message("item pasted successfully"); // show success message
          interface.show_file_list(); // update display
          interface.show_path(); // show current path
        } else if(interface.paste() == -FileOperator::E_ITEM_DOES_NOT_EXIST){
          interface.show_error("no object copied"); // if user tries to paste without copying
        } else if(interface.paste() == -FileOperator::E_ITEM_EXISTS){
          interface.show_error("object already exists"); // if object already exists
          new_filename = interface.ask_filename();
          interface.paste(new_filename);
          interface.show_file_list(); // update display
        } else {
          interface.show_error("unknown error occured"); // default error message
        }

        break;

      default:
        interface.show_error("invalid command"); // any unknown command
        interface.show_error(std::to_string(command)); // shows the key of the unknown command
        break;
    }
  }

  return 0;
}