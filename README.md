
# FileMe
Aidan Matthews a1887077, Varad Sharma a1885288

Have you ever been working in your terminal and wished you had a simple, visual way of managing your files? No more! Introducing FileMe, a simple terminal-based file manager created for Problem Solving and Software Development in Semester 2 2025.

For summary and evaluation documents, please see the [`doc`](doc) folder.
For usage, see the [`usage.md`](doc/usage.md) guide.

For building instructions, please see below:

## Dependencies

There are several dependencies required to build this project, they are as follows:
 - CMake
 - make
 - clang-format
 - doctest (installed via cmake)
 - curses (or ncurses)

The development systems used were Arch and Debian Linux on WSL. The dependencies for these platforms can be installed as follows (note this may differ for your system):
Arch Linux: `sudo pacman -S cmake make clang ncurses`
Debian Linux: `sudo apt install cmake make clang-format libncurses-dev`

## Building the Application

There are a number of make targets which can be used to build different targets. Note that they must be run from the root directory of this project. They are as follows:

### Target: build

This will build the standalone application and required dependencies, placing the result in the `build` directory.

```bash
make build
```

After running `make build` the application can be run by executing `./build/standalone/FileMe`. Note that this is the same as the `make run` build target. Note that if the `make run` target fails, try
running `make clean` prior to the `make run` command.

### Target: test

Run the application unit tests via `doctest`, reporting the results.
```bash
make test
```

### Target: format

Format the source code via `clang-format`.
```bash
make format
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
