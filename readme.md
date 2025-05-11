# PURR BROS - 2D Platform Game

![PURR BROS](assets/environment/background.png)

## Description
PURR BROS is a 2D platform game developed in C++ with Qt. The player controls a character who must navigate through various platforms to reach a final goal while collecting coins. The game records the best completion time and allows restarting at any time.

## Features
- Character control with keyboard keys (Z/Q/S/D or arrow keys)
- Realistic jump and gravity system
- Mobile and static platforms
- Coin collection
- Timer and best time recording
- Menu with "About" and "Restart" options
- Player-centered view that follows movements

## Objectives
Collect as many coins as possible as quickly as possible, and reach the treasure chest to secure the coins.

## Controls
- Movement: ZQSD keys or arrows
- Jump: Space bar
- Play again: R key

## Installation
### Prerequisites
- Qt5 (Core, Gui, Widgets)
- C++ compiler compatible with C++11
- CMake (version 3.25 or higher)

### Compilation
1. Clone the repository or download the source files
2. Create a build directory:
   ```bash
   mkdir build && cd build
3. cmake ..
4. cmake --build .


## Project Structure
.
├── CMakeLists.txt          # CMake configuration file
├── main.cpp                # Application entry point
├── MainWindow.h/cpp        # Main window management
├── MyScene.h/cpp           # Game scene management
└── assets/                 # Folder containing graphic resources
    ├── environment/        # Backgrounds and platforms
    ├── player/             # Player sprites
    └── ...

## Note about Code
The source code contains French comments and identifiers. If you have any questions about the implementation, feel free to contact me on Discord @colin503.

## Author
Colin503 - Main developer

##  License
This project is an academic work and is not intended for commercial use.

This README provides all essential information about the project, including how to compile, run, and control the game. You may modify it as needed, for example to add screenshots or additional information about game mechanics.
