# A basic UI mock-up and ideas summary

General UI ideas:

files / directories contained within the current workspace are presented as a horizontal list of file names, you can only see the items visible at the current directory level.

If a directory is navigated into, the list will refresh with the items contained within that directory.

The UI can be split into three different sections which will be represented as distinct windows within the curses environment. These are:
 1. The topmost and largest window, containing the list of files within the current working directory.
 2. A small horizontal bar describing the commands for usage of the program, in addition to an indication that a file has been selected for copying
 3. A single line for use with text commands, such as confirmation of a command or presenting a new file name when renaming a file.

At a basic level, this will look like:
```
+-------------------------------------------------------------------------+
|                                                                         | Window 1
|  [..] <<< always visible?                                               |
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
