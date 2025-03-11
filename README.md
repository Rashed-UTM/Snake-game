# Console Snake Game

A classic Snake game implemented in C++ that runs in the Windows console.

## Features
- ASCII graphics
- Wall boundaries
- Score tracking
- Progressive difficulty (speed increases as you score)
- Win condition when snake reaches sufficient length

## Controls
- **W**: Move Up
- **A**: Move Left
- **S**: Move Down
- **D**: Move Right
- **Q**: Quit game

## Requirements
- Windows Operating System
- C++ compiler (e.g., Visual Studio, MinGW)

## How to Compile and Run

### Using Visual Studio
1. Open Visual Studio
2. Create a new C++ Console Application
3. Replace the default code with the code from `snake_game.cpp`
4. Build and run the project (F5)

### Using MinGW/g++
1. Open Command Prompt or PowerShell
2. Navigate to the directory containing `snake_game.cpp`
3. Compile the code:
```
g++ snake_game.cpp -o snake_game.exe
```
4. Run the game:
```
.\snake_game.exe
```

## Game Rules
- Control the snake to eat food (*) which appears randomly on the screen
- Each food item increases your score by 10 points
- The snake grows longer each time it eats food
- The game ends if the snake hits a wall or itself
- You win if the snake reaches a length of 50 segments

## Implementation Details
- Written in C++ using Windows console APIs
- Uses simple ASCII characters for visuals
- Implements a game loop with input handling, state updates, and rendering 