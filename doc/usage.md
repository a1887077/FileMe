# FileMe Usage Guide

The following illustrates how to use the FileMe program. This is known to be working using WSL (Windows Subsystem for Linux) with Arch Linux and Debian distributions through Windows Terminal and the VSCode Terminal.

**IMPORTANT NOTE**: There is no way to undo a command!! This means that any **deletion is permanent**! While you will be asked to confirm your action when trying to delete a file, **please be careful**!

Once built, the executable will be placed in the `build/standalone` directory. For building instructions please refer to the [README](../README.md).

The program can be run as follows from the `standalone` directory.
```bash
>>> ./FileMe [DIR]
```
By default, FileMe opens with a working directory equal to the active directory from which it is invoked, to specify a specific directory, use the [DIR] argument, for example:
```bash
>>> ./FileMe ./some/other/directory
```

## Controls
The application has the following controls:

**Navigation**
 - Up Arrow - navigate up the list of files / directories
 - Down Arrow - navigate down the list of files / directories
 - Enter/Right Arrow - navigate into a directory
 - Backspace/Left Arrow - navigate out of the cuurrent directory
 - Q - Quit the program

**File / Directory Manipulation**
 - N - Create a new file with a prompt for the name
 - R - Rename a file with a prompt for the new name
 - D - Delete a file or directory with a prompt for confirmation
 - C - Select a file or directory for copying
 - P - Paste the selected file or directory

The green highlighted item is the entry which is selected for any actions, and will be modified if any of the above commands are invoked.
