#include <curses.h>
#include <fileme/manager.h>
#include <fileme/version.h>

#include <iostream>
#include <string>
#include <vector>

void printFiles(Manager& fileManager, int index) {
  // write code here to show the file in the management system
  clear();
  printw("Files in the current database (Green: shows the currently selected file):\n");
  refresh();

  std::vector<DirEntry> files = fileManager.getEntries();

  for (size_t i = 0; i < files.size(); i++) {
    DirEntry file = files[i];
    if (i == index) {
      attron(COLOR_PAIR(1));
      printw("- %s\n", file.getName().c_str());
      attroff(COLOR_PAIR(1));
    } else {
      printw("- %s\n", file.getName().c_str());
    }
  }
  printw("\n\n\nPress 'n' to create other files, press 'q' to exit\n");  // edit this later
  refresh();
}

int main(int argc, char** argv) {
  initscr();
  curs_set(0);
  keypad(stdscr, TRUE);                  // Enable arrow keys and function keys
  noecho();                              // Don't echo input
  curs_set(0);                           // Hide cursor
  // std::vector<std::string> fileStorage;  // to store all the created files
  Manager fileManager;

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
  char filename[21];  // store the name (20 characters, can change)
  int files = fileManager.numEntries();
  bool skipstart = false;
  while (true) {
    bool showFiles = false;
    if (skipstart == false) {
      if (files == 0) {
        clear();
        printw("No files in the management system, press 'n' to create a new file");
        refresh();
      } else {
        showFiles = true;
      }

      ch = getch();
      if (ch == 'q') {
        break;
      }
      if (ch == 'n') {
        clear();
        mainWindow = true;
      }
    }

    while (mainWindow == true) {
      // this is where we ask the user to input a file name
      clear();
      printw("Please Pick a file name (max 20 characters):\n");
      refresh();

      echo();            // DO NOT REMOVE
      getstr(filename);  // writes over any old name
      noecho();

      printw("You entered: %s\n", filename);
      refresh();
      // confirm message
      printw("Do you confirm? [Y/N]");
      refresh();
      int local = getch();
      if (local == 'Y') {
        mainWindow = false;
        if (fileManager.create(filename, FILE_ENTRY) == 0) {
          files++;
          showFiles = true;
        }
        break;
      }
      // else we loop back
    }

    bool breakout = false;
    if (showFiles == true) {
      int currentIndex = 0;
      clear();
      printw("File Added to system, press 'ENTER' to continue");
      refresh();
      getch();

      printFiles(fileManager, currentIndex);

      while (true) {
        ch = getch();
        if (ch == 'q') {
          breakout = true;
          break;
        }
        if (ch == 'n') {
          mainWindow = true;
          skipstart = true;
          break;
        }
        clear();
        switch (ch) {
          case KEY_UP:
            if (currentIndex != 0) {
              printFiles(fileManager, currentIndex - 1);
              currentIndex--;
            } else {
              printFiles(fileManager, 0);
            }
            break;
          case KEY_DOWN:
            if (currentIndex != fileManager.numEntries() - 1) {
              printFiles(fileManager, currentIndex);
              currentIndex++;
            } else {
              printFiles(fileManager, fileManager.numEntries() - 1);
            }
            break;
        }

        printFiles(fileManager, currentIndex);
      }
    }
    if (breakout) {
      break;
    }
  }
  // std::cout << "hello world" << std::endl;
  // printw("hi");
  // refresh();

  endwin();
  return 0;
}
