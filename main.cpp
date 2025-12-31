#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "Connector.hpp"  
using namespace sf;

int size = 56;
Sprite f[32];
bool whiteTurn = true; 
bool wK = false, bK = false, wRL = false, wRR = false, bRL = false, bRR = false;
int enPassantCol = -1;
bool isFlipped = false;
bool playAgainstCPU = true; 
enum GameState { MENU, PLAYING };
GameState currentState = MENU;
std::string position = "";

bool whiteKingMoved = false, blackKingMoved = false;
bool whiteRookL = false, whiteRookR = false; 
bool blackRookL = false, blackRookR = false;
std::string lastMove = ""; 

int board[8][8] = 
    {-1,-2,-3,-4,-5,-3,-2,-1,
     -6,-6,-6,-6,-6,-6,-6,-6,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      6, 6, 6, 6, 6, 6, 6, 6,
      1, 2, 3, 4, 5, 3, 2, 1};

Vector2f toCoord(char a, char b) {
    int x = int(a) - 97;
    int y = 7 - int(b) + 49;
    if (isFlipped) { x = 7 - x; y = 7 - y; }
    return Vector2f(x * size, y * size);
}

std::string toChessNote(Vector2f p) {
    int x = p.x / size;
    int y = p.y / size;
    if (isFlipped) { x = 7 - x; y = 7 - y; }
    std::string s = "";
    s += char(x + 97);
    s += char(7 - y + 49);
    return s;
}

bool isPathClear(int x1, int y1, int x2, int y2) {
    int dx = (x2 == x1) ? 0 : (x2 > x1 ? 1 : -1);
    int dy = (y2 == y1) ? 0 : (y2 > y1 ? 1 : -1);
    int currX = x1 + dx;
    int currY = y1 + dy;
    while (currX != x2 || currY != y2) {
        if (board[currY][currX] != 0) return false;
        currX += dx; currY += dy;
    }
    return true;
}

bool isValidPseudo(int x1, int y1, int x2, int y2, bool ignoreCheck = false);

bool isKingInCheck(bool white) {
    int kX, kY;
    int kingVal = white ? 5 : -5;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(board[i][j] == kingVal) { kY=i; kX=j; }
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if((white && board[i][j] < 0) || (!white && board[i][j] > 0)) {
                if(isValidPseudo(j, i, kX, kY, true)) return true;
            }
        }
    }
    return false;
}

bool isValidPseudo(int x1, int y1, int x2, int y2, bool ignoreCheck) {
    if (x1 == x2 && y1 == y2) return false; 
    int piece = board[y1][x1];
    int target = board[y2][x2];
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    if (!ignoreCheck) {
        if (whiteTurn && piece < 0) return false; 
        if (!whiteTurn && piece > 0) return false; 
    }
    if ((piece > 0 && target > 0) || (piece < 0 && target < 0)) return false;
    switch (abs(piece)) {
        case 6: 
            if (piece > 0) {
                if (x1 == x2 && y2 == y1 - 1 && target == 0) return true;
                if (x1 == x2 && y1 == 6 && y2 == 4 && target == 0 && board[5][x1] == 0) return true;
                if (dx == 1 && y2 == y1 - 1 && (target < 0 || (y1 == 3 && x2 == enPassantCol))) return true;
            } else {
                if (x1 == x2 && y2 == y1 + 1 && target == 0) return true;
                if (x1 == x2 && y1 == 1 && y2 == 3 && target == 0 && board[2][x1] == 0) return true;
                if (dx == 1 && y2 == y1 + 1 && (target > 0 || (y1 == 4 && x2 == enPassantCol))) return true;
            }
            return false;
        case 5:
            if (dx <= 1 && dy <= 1) return true;
            if (!ignoreCheck && dy == 0 && dx == 2) {
                if (x2 == 6) {
                    bool moved = (piece > 0) ? (wK || wRR) : (bK || bRR);
                    if (!moved && isPathClear(x1, y1, 7, y1)) return true;
                }
                if (x2 == 2) {
                    bool moved = (piece > 0) ? (wK || wRL) : (bK || bRL);
                    if (!moved && isPathClear(x1, y1, 0, y1)) return true;
                }
            }
            return false;
        case 1: if (x1 == x2 || y1 == y2) return isPathClear(x1, y1, x2, y2); return false;
        case 2: return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
        case 3: if (dx == dy) return isPathClear(x1, y1, x2, y2); return false;
        case 4: if ((x1 == x2 || y1 == y2) || (dx == dy)) return isPathClear(x1, y1, x2, y2); return false;
        default: return false;
    }
}

bool isValid(int x1, int y1, int x2, int y2) {
    if (!isValidPseudo(x1, y1, x2, y2)) return false;
    int tempTarget = board[y2][x2];
    int startPiece = board[y1][x1];
    board[y2][x2] = startPiece; board[y1][x1] = 0;
    bool safe = !isKingInCheck(startPiece > 0);
    board[y1][x1] = startPiece; board[y2][x2] = tempTarget;
    return safe;
}

void loadPosition() {
    int k = 0;
    for (int i = 0; i < 32; i++) f[i].setPosition(-100, -100);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int n = board[i][j];
            if (!n) continue; 
            int x = abs(n) - 1;
            int y = n > 0 ? 1 : 0;
            f[k].setTextureRect(IntRect(size * x, size * y, size, size));
            f[k].setPosition(toCoord(char(j + 97), char(7 - i + 49)));
            k++;
        }
    }
}

void move(std::string str) {
    int x1 = str[0] - 'a', y1 = 8 - (str[1] - '0');
    int x2 = str[2] - 'a', y2 = 8 - (str[3] - '0');
    int piece = board[y1][x1];
    if (abs(piece) == 5 && abs(x2 - x1) == 2) {
        if (x2 == 6) { board[y1][5] = board[y1][7]; board[y1][7] = 0; } 
        if (x2 == 2) { board[y1][3] = board[y1][0]; board[y1][0] = 0; } 
    }
    if (abs(piece) == 6 && x1 != x2 && board[y2][x2] == 0) board[y1][x2] = 0;
    enPassantCol = (abs(piece) == 6 && abs(y2 - y1) == 2) ? x1 : -1;
    if (piece == 5) wK = true; if (piece == -5) bK = true;
    if (x1 == 0 && y1 == 7) wRL = true; if (x1 == 7 && y1 == 7) wRR = true;
    board[y2][x2] = piece;
    if (abs(piece) == 6 && (y2 == 0 || y2 == 7)) board[y2][x2] = (piece > 0) ? 4 : -4; 
    board[y1][x1] = 0;
}

bool canMove(bool white) {
    for (int y1 = 0; y1 < 8; y1++) {
        for (int x1 = 0; x1 < 8; x1++) {
            if ((white && board[y1][x1] > 0) || (!white && board[y1][x1] < 0)) {
                for (int y2 = 0; y2 < 8; y2++) {
                    for (int x2 = 0; x2 < 8; x2++) {
                        if (isValid(x1, y1, x2, y2)) return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    RenderWindow window(VideoMode(453, 453), "Chess");
    ConnectToEngine((char*)"stockfish.exe");
    int cpuSkillLevel = 10;
    std::string levelCmd = "setoption name Skill Level value " + std::to_string(cpuSkillLevel) + "\n";
    WriteFile(pipin_w, levelCmd.c_str(), levelCmd.length(), &writ, NULL);
    Texture t1, t2;
    t1.loadFromFile("images/figures.png");
    t2.loadFromFile("images/board0.png");
    Sprite sBoard(t2);
    for (int i = 0; i < 32; i++) f[i].setTexture(t1);
    loadPosition();  
    bool isDragging = false;
    float dx = 0, dy = 0;
    Vector2f oldPos;
    int n = -1;
    Font font;
    font.loadFromFile("ARIAL.TTF");
    Text tHuman("VS HUMAN", font, 30);
    tHuman.setPosition(80, 90);
    tHuman.setFillColor(Color::White);
    Text tCPU("VS COMPUTER", font, 30);
    tCPU.setPosition(65, 315);
    tCPU.setFillColor(Color::White);
    Text tGameOver("", font, 50);
    tGameOver.setFillColor(Color::Red);
    tGameOver.setStyle(Text::Bold);
    bool gameEnded = false;
    while (window.isOpen()) {
        Vector2i pos = Mouse::getPosition(window);
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (currentState == MENU) {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    if (pos.y < 226) playAgainstCPU = false;
                    else playAgainstCPU = true;
                    currentState = PLAYING;
                }
            } else {
                if (playAgainstCPU && !whiteTurn && !isDragging && !gameEnded) {
                    std::string engineMove = getNextMove(position);
                    if (engineMove != "error") {
                        move(engineMove);
                        position += engineMove + " ";
                        whiteTurn = true;
                        loadPosition();
                    }
                }
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left && !gameEnded) {
                    for (int i = 0; i < 32; i++) {
                        if (f[i].getGlobalBounds().contains(pos.x, pos.y)) {
                            isDragging = true; n = i;
                            dx = pos.x - f[i].getPosition().x;
                            dy = pos.y - f[i].getPosition().y;
                            oldPos = f[i].getPosition();
                        }
                    }
                }
                if (e.type == Event::MouseButtonReleased && e.mouseButton.button == Mouse::Left && n != -1) {
                    isDragging = false;
                    Vector2f p = f[n].getPosition() + Vector2f(size / 2, size / 2);
                    Vector2f newPos = Vector2f(size * int(p.x / size), size * int(p.y / size));
                    int ox = (isFlipped ? 7 - int(oldPos.x / size) : int(oldPos.x / size));
                    int oy = (isFlipped ? 7 - int(oldPos.y / size) : int(oldPos.y / size));
                    int nx = (isFlipped ? 7 - int(newPos.x / size) : int(newPos.x / size));
                    int ny = (isFlipped ? 7 - int(newPos.y / size) : int(newPos.y / size));
                    if (isValid(ox, oy, nx, ny)) {
                        std::string str = toChessNote(oldPos) + toChessNote(newPos);
                        move(str);
                        position += str + " ";
                        whiteTurn = !whiteTurn;
                        if (!playAgainstCPU) isFlipped = !isFlipped;
                        loadPosition();
                        if (!canMove(whiteTurn)) {
                            gameEnded = true;
                            tGameOver.setString(isKingInCheck(whiteTurn) ? "CHECKMATE!" : "STALEMATE!");
                            FloatRect r = tGameOver.getLocalBounds();
                            tGameOver.setOrigin(r.left + r.width/2, r.top + r.height/2);
                            tGameOver.setPosition(453/2, 453/2);
                        }
                    } else f[n].setPosition(oldPos);
                    n = -1;
                }
            }
        }
        if (isDragging && n != -1) f[n].setPosition(pos.x - dx, pos.y - dy);
        window.clear();
        window.draw(sBoard);
        if (currentState == MENU) {
            RectangleShape h(Vector2f(453, 226)); h.setFillColor(Color(0, 0, 0, 150));
            window.draw(h); window.draw(tHuman);
            RectangleShape c(Vector2f(453, 226)); c.setPosition(0, 227); c.setFillColor(Color(50, 50, 50, 150));
            window.draw(c); window.draw(tCPU);
        } else {
            for (int i = 0; i < 32; i++) window.draw(f[i]);
            if (gameEnded) {
                RectangleShape o(Vector2f(453, 453)); o.setFillColor(Color(0, 0, 0, 180));
                window.draw(o); window.draw(tGameOver);
            }
        }
        window.display();
    }
    return 0;
}