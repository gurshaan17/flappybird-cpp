# Flappy Bird Clone

This is a simple Flappy Bird clone implemented in C++ using the SFML (Simple and Fast Multimedia Library) framework. The game features a bird that the player can control to navigate through pipes while avoiding collisions.

## Features

- **Gravity and Jump Mechanics**: The bird falls due to gravity and can jump when the space key is pressed.
- **Pipes**: Randomly generated pipes that the bird must navigate through.
- **Score Tracking**: The score increases each time the bird successfully passes through a pipe.
- **Game Over State**: The game ends when the bird collides with the ground or a pipe, with an option to restart.

## Requirements

- C++
- SFML (Simple and Fast Multimedia Library)

## Running the Game Locally

1. **Clone the Repository**:
   Clone the repository to your local machine using Git:
   ```bash
   git clone github.com/gurshaan17/flappybird-cpp
   cd flappy-bird
   ```

2. **Install SFML**:
   Make sure you have SFML installed on your system. You can install it using a package manager. For example, on macOS, you can use Homebrew:
   ```bash
   brew install sfml
   ```

3. **Build the Game**:
   Use the following command to compile the game. Make sure to adjust the include and library paths according to your SFML installation:
   ```bash
   g++ -std=c++17 -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib game.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
   ```

4. **Run the Game**:
   After building, you can run the game with:
   ```bash
   ./game
   ```
