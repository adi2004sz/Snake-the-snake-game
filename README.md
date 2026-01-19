# 🐍 Snake - The Classic Snake Game

A modern implementation of the classic Snake game built with C++ and raylib. Navigate the snake, eat apples to grow longer, and avoid collisions with yourself and the walls as you rack up your score!

![Snake Gameplay](Images/game_demo.png)

## 📋 Table of Contents

- [Features](#-features)
- [Demo](#-demo)
- [Technologies](#-technologies)
- [Prerequisites](#-prerequisites)
- [Installation](#-installation)
- [How to Build](#-how-to-build)
- [How to Play](#-how-to-play)
- [Controls](#-controls)
- [Game Rules](#-game-rules)
- [Project Structure](#-project-structure)
- [Contributing](#-contributing)
- [License](#-license)

---

## ✨ Features

- 🎮 **Smooth Gameplay** - Grid-based movement with smooth animations
- 🍎 **Apple Collection** - Eat apples to grow your snake and increase your score
- 📊 **Score Tracking** - Real-time score display as you play
- 🎨 **Clean UI** - Menu system with start and game-over screens
- ⏸️ **Pause/Resume** - Take a break whenever you need
- 🔄 **Quick Restart** - Instantly restart after game over
- 💚 **Custom Graphics** - Beautiful color scheme and custom apple sprite
- 🎯 **Collision Detection** - Smart collision system for walls and self-collision

## 🎮 Demo

Here's what the game looks like in action:

![Snake Gameplay](Images/game_demo.png)

*Navigate your snake, collect apples, and watch your score grow!*

## 🛠 Technologies

This project is built using:

- **C++** - Core game logic and programming
- **raylib** - Graphics rendering and game framework
- **MSYS2/MinGW-w64** - Build toolchain for Windows

## 📦 Prerequisites

Before you begin, ensure you have the following installed:

- **MSYS2** with MinGW-w64 toolchain
- **raylib** library (installed via MSYS2)
- **g++** compiler
- **Git** (for cloning the repository)

### Installing raylib on Windows (MSYS2)

```bash
# Open MSYS2 UCRT64 terminal
pacman -S mingw-w64-ucrt-x86_64-raylib
pacman -S mingw-w64-ucrt-x86_64-gcc
```

## 📥 Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/Snake-the-snake-game.git
   cd Snake-the-snake-game
   ```

2. **Ensure raylib is installed** (see Prerequisites section)

3. **Build the project** (see next section)

## 🔨 How to Build

### Option 1: Using VS Code Task (Recommended)

If you're using VS Code, simply use the build task:

1. Press `Ctrl+Shift+B` (or `Cmd+Shift+B` on Mac)
2. Select "build raylib project"

### Option 2: Manual Build

Open your terminal and run:

```bash
g++ -g main.cpp -o main.exe -I C:/msys64/ucrt64/include -L C:/msys64/ucrt64/lib -lraylib -lwinmm -lgdi32 -lopengl32
```

**Note:** Adjust the include and library paths if your MSYS2 installation is in a different location.

## 🎯 How to Play

1. **Run the game**
   ```bash
   ./main.exe
   ```

2. **Start playing**
   - Click "Start Game" on the main menu
   - Use arrow keys or WASD to control your snake
   - Collect apples to grow and increase your score
   - Avoid hitting walls or your own body

3. **Game Over**
   - When you collide, a game-over screen appears
   - View your final score
   - Press SPACE to restart or ESC to exit

## 🎮 Controls

| Action          | Key(s)          |
| --------------- | --------------- |
| Move Up         | ↑ or W          |
| Move Down       | ↓ or S          |
| Move Left       | ← or A          |
| Move Right      | → or D          |
| Pause / Resume  | SPACE           |
| Restart         | SPACE (on Game Over) |
| Exit            | ESC             |

## 📖 Game Rules

- 🐍 The snake starts with 3 segments
- 🍎 Each apple eaten adds one segment to the snake
- 📈 Your score increases with each apple collected
- 💥 The game ends if you:
  - Hit the boundary walls
  - Collide with your own body
- ⚡ You cannot reverse direction instantly (e.g., can't go directly from right to left)

## 📁 Project Structure

```
Snake-the-snake-game/
│
├── main.cpp           # Main game source code
├── main.exe           # Compiled executable
├── README.md          # Project documentation
│
├── Graphics/          # Game assets
│   └── apple.png      # Apple sprite texture
│
└── Images/            # Documentation images
    └── game_demo.png  # Gameplay screenshot
```

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. Fork the repository
2. Create a new branch (`git checkout -b feature/AmazingFeature`)
3. Make your changes
4. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
5. Push to the branch (`git push origin feature/AmazingFeature`)
6. Open a Pull Request

### Ideas for Contributions

- Add difficulty levels (speed variations)
- Implement high score system with persistence
- Add sound effects and background music
- Create power-ups (speed boost, invincibility, etc.)
- Add different game modes (timed mode, obstacle mode)
- Improve graphics with animations

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

**Enjoy the game! 🐍🎮**

*If you found this project helpful or fun, please consider giving it a ⭐ on GitHub!*
