# Voltomino

A basic Tetris clone that runs in the Windows terminal, written in C++ as a learning project.

The game is rendered with ANSI escape codes (alternate screen buffer and box-drawing
characters for the border) and reads keyboard input through the Win32 Console API, no external libraries or game frameworks.

## Features

- Falling tetromino pieces with gravity
- Move left/right, rotate, and soft drop
- Line clearing when a full row is filled
- Next piece preview
- Game over when the stack reaches the top
- Score calculated for row breaks
- After Game Over game automatically restarts, its infinite

## Controls

| Key | Action |
| --- | ------ |
| ← / → | Move piece |
| ↑ | Rotate piece |
| ↓ | Soft drop |

## Building

Requires **Windows** and **Visual Studio** (C++20, MSVC v145 toolset).

1. Open `Volt.slnx` in Visual Studio.
2. Select a configuration (Debug/Release, x64).
3. Build and run (F5).

For best results, run the game in [Windows Terminal](https://aka.ms/terminal),
which has full support for the ANSI escape sequences used for rendering.

## Project structure

All sources live in `Volt/src/`:

| File | Purpose |
| ---- | ------- |
| `main.cpp` | Entry point, starts the `Game` |
| `game.h/cpp` | Main game loop and clock (delta time) |
| `game_manager.h/cpp` | Game rules: piece spawning, input handling, line clears, game over |
| `game_object.h/cpp` | Base class for everything in the scene (transform, mesh, collider, rigidbody) |
| `voltomino.h/cpp` | The tetromino piece object |
| `object_registry.h/cpp` | Registry of active game objects |
| `physics.h/cpp` | Gravity, movement, and collision handling |
| `renderer.h/cpp` | ANSI-based console rendering |
| `input.h/cpp` | Keyboard input via the Win32 Console API |
| `window_manager.h/cpp` | Console handles (output/input) and screen buffer setup |

## Notes

This is a learning project. It intentionally uses **raw pointers** (`new`/`delete`)
for managing pieces and game objects instead of smart pointers, as an exercise in
manual memory management and object lifetimes.
