# Termi-Rex

A terminal-based note-taking application built with C++ and FTXUI. Planned features include SQLite storage, keyboard-driven note editing, search, text import/export, and AI question answering grounded in your notes.


## Prerequisites

- A C++23-compatible compiler
- CMake 3.15+
- Make

## Build and Run

```sh
git clone https://github.com/RexTool/termi-rex.git
cd termi-rex

make all
```

After editing the code:

```sh
make build run
```

## Clean

Remove generated build files:

```sh
make clean
```

Run `make configure` again before rebuilding.