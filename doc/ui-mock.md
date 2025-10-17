# A basic UI mock-up and ideas summary

General UI ideas:

files / directories contained within the current workspace are presented as a vertical list of file names, you can only see the items visible at the current directory level.

If a directory is navigated into, the list will refresh with the items contained within that directory.

The UI can be split into three different sections which will be represented as distinct windows within the curses environment. These are:
 1. The topmost and largest window, containing the list of files within the current working directory.
 2. A small horizontal bar describing the commands for usage of the program, in addition to an indication that a file has been selected for copying
 3. A single line for use with text commands, such as confirmation of a command or presenting a new file name when renaming a file.

At a basic level, this will look like:
```
+-------------------------------------------------------------------------+
|                                                                         | Window 1
|  [..] <<< always visible? (if selected move to the previous directory)  |
|  file.c                                                                 |
|  file.h     <<< highlight currently active entry?                       |
|  directory/                                                             |
|  zeee.bee                                                               |
|  ...  <<< indicating that one should scroll to seem more items          |
|                                                                         |
|=========================================================================|
| [N] - New File   [R] - Rename File   [D] - Delete File  [C] - Copy      | Window 2
| [P] - Paste      [ENTR] - Select Directory  [BKSP] - Previous Directory |
|=========================================================================|
| >>>                                                                     | Window 3
+-------------------------------------------------------------------------+
```
A minimum terminal size of 80 x 40 characters will be the goal to give sufficient space for information.

Possible command window messages:
 - Confirm Deletion? [Y/n]: 
 - Enter new name: 
 - Operation not possible. << make red?

Highlight `[C]` when an item is selected to be copied?

If one scrolls down a list which is too large to fit in the list, move the three-dot `...` indicator to the start of the list (i.e. a three-dot indicator can exist on either end of the list if there is spill out of the window). Perhaps the visible file entries should be in their own curses window?

## Window Sizes
Sizes of the curses windows given the above assumption for layout.

All windows will have a width which is the max the terminal can provide, aiming to enforce the minimum 80 character width (display warning if the width is < 80?).

Windows will have a top/bottom padding to ensure sufficient partitioning. This padding will be one line, and can be taken up with a separating pattern as in the case of the control menu.

File list height: At minimum height of 40 lines this will give a window height of 33 lines. This height will increase with available terminal space.

Command list height: 2 lines. Do not worry about distribution horizontally

Command window height: 1 line. Should we have a command prompt (`>>>`, etc)?
