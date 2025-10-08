
# FileMe

A simple terminal-based file manager.

## Usage
The application has the following controls:
**Navigation**
 - Up Arrow - navigate up the list of files / directories
 - Down Arrow - navigate down the list of files / directories
 - Enter - navigate into a directory
 - Backspace - navigate into the previous directory (if possible)

**File / Directory Manipulation**
 - N - Create a new file with a prompt for the name
 - R - Rename a file with a prompt for the new name
 - D - Delete a file or directory with a prompt for confirmation
 - C - Select a file or directory for copying
 - P - Paste the selected file or directory

## Dependencies

There are several dependencies required to build this project, they are as follows:
 - CMake
 - make
 - clang-format
 - doctest (installed via cmake)
 - curses (or ncurses)

## Building the Application

There are a number of make targets which can be used to build different targets. These are as follows:

### Target: build

This will build the standalone application and required dependencies, placing the result in the `build` directory.

```bash
make build
```

After running `make build` the application can be run by executing `./build/fileme`. Note that this is the same as the `make run` build target.

### Target: test

Run the application unit tests via doctest, reporting the results.
```bash
make test
```

### Target: format

Format the source code via `clang-format`.
```bash
make test
```

### Target: all

Build all source code and tests
```bash
make all
```

### Target: clean

Clean (remove) the build directory and it's contents.
```bash
make clean
```
