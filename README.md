# Bubble Shooter (C++ CLI Prototype)

This repository contains the initial C++ implementation of a Bubble Shooter style game.
The current version is an early command-line prototype that generates and prints a random game board.

## Current Status

- Language: C++17
- Build system: CMake (minimum 3.20)
- Executable target: `game`
- Platform: Linux (works on other platforms with a C++17 compiler)

## What Is Implemented

- `8x8` board generation
- Random bubble colors at startup
- Board rendering in the terminal with row/column headers

Current color mapping:

- `A` = Red
- `B` = Green
- `C` = Blue
- `D` = Yellow
- `E` = Purple
- `.` = Empty/None

## Project Structure

```text
.
|-- CMakeLists.txt
|-- src/
|   |-- main.cpp
|   |-- game.h
|   |-- game.cpp
|   `-- board/
|       |-- board.hpp
|       |-- board.cpp
|       `-- random.hpp
`-- README.md
```

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/game
```

Example output shape:

```text
Simple Candy Crush CLI (Phase 1-2)
Initial board:
	0 1 2 3 4 5 6 7
0 A C B E D A C B
...
```

## Next Milestones

- Add player input and shooting mechanics
- Detect connected bubble groups
- Remove matched groups and apply gravity/fall logic
- Add win/lose conditions and scoring
- Improve terminal visualization (or move to a graphical frontend)

## Notes

The `build/` directory is intentionally ignored by Git via `.gitignore`.