# Bubble Shooter (C++ CLI Prototype)

This repository contains the initial C++ implementation of a Bubble Shooter style game.
The current version is an early command-line prototype that generates and prints a random game board.

## Current Status

- Language: C++17
- Build system: CMake (minimum 3.20)
- Executable target: `game`
- Platform: Linux (works on other platforms with a C++17 compiler)

## What Is Implemented


The following features are currently implemented:

- **Board generation and printing:** The game generates a random board and prints it to the terminal.
- **Player input loop:** The player can enter an angle (20-160) or type 'exit' to quit.
- **Projectile shooting:** Projectiles are launched at the chosen angle and attempt to attach to the board, with basic collision and ceiling logic.
- **Color management:** The `ColorManager` tracks available bubble colors and their counts.
- **Basic group clearing:** The board supports clearing connected groups of bubbles (minimum group size logic present).

Current color mapping:

Current color mapping:

- Red
- Green
- Blue
- Yellow
- Purple
- Empty/None

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