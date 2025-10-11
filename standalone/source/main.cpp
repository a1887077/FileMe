#include <fileme/version.h>
#include <curses.h>
#include <iostream>
#include <string>
#include "fileme/fileOperator.h"
#include <vector>
#include <string>
void printFiles(std::vector<std::string> vector, int index){
      // write code here to show the file in the management system
      clear();
      printw("Files in the current database (Green: shows the currently selected file):\n");
      refresh();
      
      for(size_t i = 0; i < vector.size(); i++){
        if(i == index){
          attron(COLOR_PAIR(1));
          printw("- %s\n", vector[i].c_str());
          attroff(COLOR_PAIR(1));
        } else {
          printw("- %s\n", vector[i].c_str());
        }
      }
      printw("\n\n\nPress 'n' to create other files, press 'q' to exit\n"); // edit this later
      refresh();
}


int main(int argc, char** argv) {
  initscr();
  curs_set(0);
  keypad(stdscr, TRUE);   // Enable arrow keys and function keys
  noecho();               // Don't echo input
  curs_set(0);            // Hide cursor
  std::vector<std::string> fileStorage; // to store all the created files
  FileOperator console;
  start_color();
  use_default_colors();
  init_pair(1, COLOR_GREEN, -1);

  // intro message (add whatever else you want btw)
  printw("Welcome to file manager, press 'ENTER' to start");
  refresh();
  getch();

  int ch;
  int index = 0;
  
  bool mainWindow = false;
  char filename[21]; // store the name (20 characters, can change)
  int files = 0;
  bool skipstart = false;
  while(true){
    bool showFiles = false;
    if(skipstart == false){
      if(files == 0){
        clear();
        printw("No files in the management system, press 'n' to create a new file");
        refresh();
      }
      ch = getch();
      if(ch == 'q'){
        break;
      }
      if(ch == 'n'){
        clear();
        mainWindow = true;
      }
    }

    while(mainWindow == true){
      // this is where we ask the user to input a file name
      clear();
      printw("Please Pick a file name (max 20 characters):\n");
      refresh();

      echo(); // DO NOT REMOVE
      getstr(filename); // writes over any old name
      noecho();

      printw("You entered: %s\n", filename);
      refresh();
      // confirm message
      printw("Do you confirm? [Y/N]");
      refresh();
      int local = getch();
      if(local == 'Y'){
        mainWindow = false;
        files++;
        fs::path new_file = filename;
        int answer = console.create(new_file); // create the file
        if(answer == 0){
          fileStorage.push_back(new_file.string());
          showFiles = true;
        }
        break;
      }
      // else we loop back
    }


    bool breakout = false;
    if(showFiles == true){
      int currentIndex = 0;
      clear();
      printw("File Added to system, press 'ENTER' to continue");
      refresh();
      getch();

      printFiles(fileStorage, currentIndex);

      while(true){
        ch = getch();
        if(ch == 'q'){
          breakout = true;
          break;
        }
        if(ch == 'n'){
          mainWindow = true;
          skipstart = true;
          break;
        }
        clear();
        switch(ch){
          case KEY_UP:
            if(currentIndex != 0){
              printFiles(fileStorage, currentIndex - 1);
              currentIndex--;
            } else {
              printFiles(fileStorage, 0);
            }
            break;
          case KEY_DOWN:
            if(currentIndex != fileStorage.size() - 1){
              printFiles(fileStorage, currentIndex);
              currentIndex++;
            } else {
              printFiles(fileStorage, fileStorage.size() - 1);
            }
            break;
        }

        printFiles(fileStorage, currentIndex);
      }      
    }
    if(breakout){
      break;
    }


  }
  // std::cout << "hello world" << std::endl;
  // printw("hi");
  // refresh();

  endwin();
  return 0;
}

