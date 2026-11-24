# Termi-Rex

A terminal-based note-taking application built with C++ and FTXUI. Its Vim-style editor opens and saves plain-text files. Planned features include SQLite storage, search, text import/export, and AI question answering grounded in your notes.


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

## Editing a file

`make run` opens `note.txt` in the current directory. To edit a different file, run
`./build/bin/app path/to/file.txt`. A new file is created on the first save.

- Start in Normal mode. Use `h` (left), `j` (down), `k` (up), and `l` (right).
- Press `i` to insert before the cursor or `a` to append after it. Type normally, then press `Esc` to return to Normal mode. In Normal mode, `x` deletes the character under the cursor.
- Type `:w` to save, `:q` to quit, or `:wq` to do both. Press `Enter` to run the command. Use `:q!` to discard unsaved edits.

## Clean

Remove generated build files:

```sh
make clean
```

Run `make configure` again before rebuilding.
