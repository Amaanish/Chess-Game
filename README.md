#  Chess Game in C++

A chess game developed in **C++** with **Stockfish** engine integration, featuring both console-based and advanced graphical gameplay. This project evolved from a basic chess implementation to a fully-featured game with AI, move validation, and intuitive drag-and-drop controls.

---

##  Project Overview

This chess implementation demonstrates **two major versions** showing clear progression in software development:

- **V1 (157 lines)**: Basic foundation with simple drag-and-drop and move tracking
- **V2 (Current)**: Full-featured game with complete chess rules, AI opponent, turn management, and game state detection

The project integrates **Stockfish**—one of the strongest open-source chess engines—to provide challenging AI opponents and comprehensive move validation.

---

##  Features

### Core Features (Both Versions)
- **Graphical Interface**: SFML-based drag-and-drop piece movement
- **Stockfish Integration**: Powerful chess engine for validation and AI
- **Game State Management**: Persistent position tracking and move history
- **Modular Architecture**: Clean, organized codebase

### V2 Advanced Features
- **Complete Chess Rules**: Full implementation of standard chess rules including:
  - **Castling**: Both kingside and queenside with proper move validation
  - **En Passant**: Correct en passant capture mechanics
  - **Pawn Promotion**: Automatic promotion to Queen on reaching back rank
  - **Check & Checkmate Detection**: Real-time game status monitoring
  - **Stalemate Recognition**: Proper game termination conditions

- **Game Modes**:
  - **Human vs Human**: Two-player gameplay with board flipping
  - **Human vs Computer**: Play against Stockfish AI with adjustable skill levels

- **Dual Interface**:
  - Full graphical board with piece sprites
  - Main menu for game mode selection
  - Game over screen with match results

- **Advanced Move Validation**:
  - Pseudo-legal move generation
  - King safety checks (can't move into check)
  - Path clarity validation for sliding pieces
  - Turn-based gameplay enforcement

---

##  Version Comparison

| Feature | V1 | V2 |
|---------|----|----|
| Basic drag-and-drop | ✅ | ✅ |
| Move tracking | ✅ | ✅ |
| Castling | ❌ | ✅ |
| En Passant | ❌ | ✅ |
| Pawn Promotion | ❌ | ✅ |
| Check Detection | ❌ | ✅ |
| Checkmate Detection | ❌ | ✅ |
| Game Mode Menu | ❌ | ✅ |
| Human vs AI | ❌ | ✅ |
| Human vs Human | ❌ | ✅ |
| Board Flipping | ❌ | ✅ |
| Turn Management | Basic | Advanced |
| Lines of Code | 157 | 400+ |

---

##  Project Structure

```
Chess-Game/
├── main.cpp                           # Current game implementation (V2)
├── Connector.hpp                      # Stockfish engine interface/connector
├── stockfish.exe                      # Stockfish engine executable
├── board_state.txt                    # Game state persistence
├── Previous Versions/V1/              # Original version (157 lines)
│   └── main.cpp
├── images/
│   ├── figures.png                    # Chess piece sprites
│   └── board0.png                     # Chess board texture
├── ARIAL.TTF                          # Font for UI text
├── SFML Libraries (Windows)
│   ├── sfml-graphics-2.dll            # Graphics rendering
│   ├── sfml-window-2.dll              # Window management
│   ├── sfml-system-2.dll              # Core system functions
│   └── openal32.dll                   # Audio support
└── README.md
```

---

##  Getting Started

### Prerequisites

- **C++ Compiler**: GCC (g++) or equivalent
- **SFML Library**: Simple and Fast Multimedia Library
- **SFML DLL Files** (Windows): `sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll`, `openal32.dll`
- **Stockfish**: Chess engine executable (included)

### Installation

#### On Linux/Unix:

```bash
sudo apt update
sudo apt install g++
sudo apt install libsfml-dev
```

#### On Windows:

- Install MinGW-w64 or Visual Studio C++ tools
- Download SFML from [sfml-dev.org](https://www.sfml-dev.org/)
- Add SFML to your compiler include/lib paths
- **Required DLL files** (included in repository):
  - `sfml-graphics-2.dll` - Graphics rendering library
  - `sfml-window-2.dll` - Window and input management
  - `sfml-system-2.dll` - Core system library
  - `openal32.dll` - Audio support library
  
  Place these DLL files in the same directory as the executable for the game to run.

#### On macOS:

```bash
brew install gcc
brew install sfml
```

---

## 🔧 Compilation & Execution

### Compile from Source

```bash
# Compile with SFML libraries
g++ -c main.cpp -o main.o
g++ main.o -o chess_game -lsfml-graphics -lsfml-window -lsfml-system

# Or use the pre-compiled executable
./chess_game.exe
```

### Running the Game

```bash
./chess_game
```

The game will launch with a main menu where you can select your game mode.

---

##  How to Play

### Main Menu
- **Top Half**: Click to play **Human vs Human** (local multiplayer)
- **Bottom Half**: Click to play **Human vs Computer** (against Stockfish AI)

### Gameplay
1. **Select a Piece**: Click on one of your pieces to select it
2. **Drag to Move**: Hold and drag the piece to your target square
3. **Release to Execute**: The move is validated and executed
4. **Alternate Turns**: Players alternate turns (board flips in Human vs Human mode)
5. **Game End**: Game ends on checkmate or stalemate

### Special Moves
- **Castling**: Click and drag your king two squares toward a rook (if eligible)
- **En Passant**: Pawn captures are automatic if conditions are met
- **Pawn Promotion**: Promoted automatically to Queen upon reaching the back rank

---

##  Stockfish Integration

### Connector.hpp
Handles all communication with Stockfish engine:
- Initializes engine process
- Sends board positions in standard notation
- Retrieves best move recommendations
- Provides skill level configuration

### Engine Features
- **AI Opponent**: Plays at configurable skill levels (1-20)
- **Move Validation**: All moves validated against chess rules
- **Position Analysis**: Evaluates moves and provides game assessment
- **Difficulty Control**: `cpuSkillLevel` variable adjusts AI strength (default: 10)

---

##  Chess Rules Implementation

### Move Validation System

**Pseudo-Legal Move Validation**
```cpp
bool isValidPseudo(int x1, int y1, int x2, int y2)
```
Validates individual piece movement rules:
- Pawn: Single forward (double on first move), diagonal captures, en passant
- Knight: L-shaped moves (2+1 squares)
- Bishop: Diagonal movement with clear path
- Rook: Horizontal/vertical movement with clear path
- Queen: Combined rook + bishop movement
- King: One square in any direction, castling (2 squares with rook)

**Legal Move Validation**
```cpp
bool isValid(int x1, int y1, int x2, int y2)
```
Ensures move is legal by:
1. Checking pseudo-legality
2. Simulating the move
3. Verifying king is not in check after move
4. Restoring board state

**Game State Detection**
```cpp
bool canMove(bool white)  // Can side move?
bool isKingInCheck(bool white)  // Is king under attack?
```

### Board Representation
- 8x8 integer array representing piece positions
- Positive values: White pieces (1-6)
- Negative values: Black pieces (-1 to -6)
- Piece values: 1=Rook, 2=Knight, 3=Bishop, 4=Queen, 5=King, 6=Pawn

---

##  Version History

### V1 (Previous Implementation)
- Basic drag-and-drop mechanics
- Simple move tracking with position string
- Limited move validation
- Single-player experience
- ~157 lines of code

### V2 (Current - Main Branch)
- Complete chess rules implementation
- Turn-based gameplay with proper enforcement
- Game mode selection menu
- Checkmate/stalemate detection
- AI opponent with Stockfish
- Board flip for two-player mode
- ~400+ lines with advanced features

---

## 🛠️ Key Technical Concepts

### Architecture Highlights

**Separation of Concerns**
- `main.cpp`: Game loop, UI, and user input
- `Connector.hpp`: Stockfish communication layer
- Chess logic: Integrated validation system

**Data Structures**
- Board representation as 2D integer array
- Sprite array for visual piece management
- Position string for move history

**Algorithm Implementation**
- Depth-first path validation for sliding pieces
- Recursive check detection
- State simulation for legality verification
- Efficient piece location tracking

**UI/UX Features**
- Menu state management
- Real-time piece dragging
- Visual feedback (game over overlay)
- Board orientation flipping

---

## 🐛 Known Limitations

- Requires Stockfish executable in game directory
- Limited to single game session per run
- No game save/load functionality beyond move position
- No move undo feature (except restart)
- AI doesn't provide analysis beyond move selection

---

## 🚀 Future Enhancement Ideas

- **Features**:
  - Game history and replay functionality
  - Opening book integration for stronger AI
  - Multiple difficulty tiers with time controls
  - FEN position import/export
  - Move notation display (algebraic notation)
  - Sound effects and animations

- **Improvements**:
  - Network multiplayer support
  - Endgame tablebase integration
  - Position evaluation display
  - Move suggestion hints
  - Tournament mode

- **UX**:
  - Undo/redo functionality
  - Move highlighting
  - Captured pieces display
  - Clock timers for blitz/rapid games
  - Settings menu for customization

---


##  Author

**Amaanish**

Questions? Suggestions? Feel free to open an issue or contribute improvements!

---

##  Acknowledgments

- **Stockfish Team**: World's strongest open-source chess engine
- **SFML Community**: Excellent multimedia library documentation
- **Chess Programming Community**: Resources on move generation and validation
- **YouTube Tutorial**: (https://www.youtube.com/watch?v=_4EuZI8Q8cs)

---

**Last Updated**: December 2025  
**Current Version**: V2 (Advanced) | V1 Available in Previous Versions folder
