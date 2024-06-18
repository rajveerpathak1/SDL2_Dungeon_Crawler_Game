# SDL2 Dungeon Crawler

This project implements a simple dungeon crawler game using SDL2 and C++. The game generates a dungeon with rooms, enemies, items, treasures, and an exit. The player navigates through the dungeon, collects items and treasures, avoids enemies, and reaches the exit to regenerate a new dungeon.

## Features

- Player Movement: Use arrow keys (UP, DOWN, LEFT, RIGHT) to move the player character.
- Collision Detection: Check collisions between the player and enemies, items, treasures, and the exit.
- Health Bar: Display and update the player's health.
- Inventory: Manage and display collected items in the player's inventory.
- Random Dungeon Generation: Dynamically generate a new dungeon layout with random rooms, enemies, items, treasures, and exit positions.

## Requirements

- SDL2 Library
- SDL2_image Library
- C++ Compiler (supporting C++11)

## Installation

1. Clone the Repository:
   git clone https://github.com/your_username/sdl2-dungeon-crawler.git
   cd sdl2-dungeon-crawler

2. Build the Project:
   - Ensure SDL2 and SDL2_image libraries are installed and accessible.
   - Compile the project using your preferred C++ compiler. Example:
     g++ main.cpp -o dungeon_crawler -lSDL2 -lSDL2_image

## Usage

- Controls:
  - Use Arrow Keys to move the player character.
  - Collect items and treasures by moving over them.
  - Avoid enemies to maintain player health.
  - Reach the exit to regenerate a new dungeon layout.

- Running the Game:
  - Execute the compiled executable after building:
    ./dungeon_crawler

## Contributing

Contributions to enhance and expand this dungeon crawler game are welcome! Here are a few ways you can contribute:

- Improve gameplay mechanics (e.g., combat, item interaction).
- Enhance graphics and visual effects.
- Optimize code and improve performance.
- Fix bugs and issues reported.

To contribute:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature`)
3. Make your changes.
4. Commit your changes (`git commit -am 'Add new feature'`)
5. Push to the branch (`git push origin feature`)
6. Create a new Pull Request.

---
