// ======================================
// 🎮 2-Player Shooter Game (Refactored + Timer & Score Fixes)
// ======================================
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

// ======================================
// 🛠️ CONFIGURATION & CONSTANTS
// ======================================
const int WIDTH = 30, HEIGHT = 25, MAX_BULLETS = 34, MAX_BOMBS = 6;

const string COLOR_GREEN   = "\033[1;32m";
const string COLOR_RED     = "\033[1;31m";
const string COLOR_BLUE    = "\033[1;34m";
const string COLOR_MAGENTA = "\033[1;35m";
const string COLOR_CYAN    = "\033[1;36m";
const string COLOR_YELLOW  = "\033[1;33m";
const string COLOR_RESET   = "\033[0m";

char PLAYER_A_CHAR = '^';
char PLAYER_B_CHAR = 'v';
char BULLET_CHAR   = '|';
char BOMB_CHAR     = '*';
char EXPLOSION_CHAR = '#';

// ======================================
// 📦 STRUCTURES
// ======================================
bool Aalive = true, Balive = true;
int playerAScore = 0, playerBScore = 0;

struct Bullet {
    int x, y, direction;
    bool active = false;
    char owner; // 'A' or 'B'
} bullets[MAX_BULLETS];

struct Side_Bomb {
    int x, y;
};

struct Bomb {
    int x, y, direction, drift;
    bool active = false;
    Side_Bomb Side_Bombs[3];
    string color;
    bool exploding = false;
    int explodeFrame = 0;
} bombs[MAX_BOMBS];

int playerAX = WIDTH / 4, playerAY = HEIGHT - 1, prevAX = playerAX, prevAY = playerAY;
int playerBX = (WIDTH * 3) / 4, playerBY = HEIGHT - 1, prevBX = playerBX, prevBY = playerBY;
int frameCount = 0;

steady_clock::time_point startTime;
int gameDurationSec = 0;
bool gameTimeOver = false;

// ======================================
// 🎵 SOUND EFFECTS (Less irritating)
// ======================================
void playShootSound()         { Beep(800, 60); }
void playBombDestroyedSound() { Beep(650, 80); }
void playPlayerHitSound()     { Beep(500, 150); }
void playHealthPickupSound()  { Beep(1000, 100); }
void playGameOverSound() {
    Beep(400, 200);
    Sleep(100);
    Beep(300, 300);
}

// ======================================
// 🎨 VISUAL HELPERS
// ======================================
string getRandomColor() {
    string colors[] = {
        COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
        COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN
    };
    return colors[rand() % 6];
}

string getPulseColor(int frame) {
    string palette[] = {COLOR_RED, COLOR_YELLOW, COLOR_MAGENTA};
    return palette[frame % 3];
}

void hideCursor() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    SetConsoleCursorInfo(h, &info);
}

// ======================================
// 💣 BOMB LOGIC
// ======================================
void spawnBomb() {
    for (auto& b : bombs) {
        if (!b.active && !b.exploding) {
            b = {rand() % (WIDTH - 2) + 1, 0, 1, rand() % 3 - 1, true};
            b.color = getRandomColor();
            b.exploding = false;
            b.explodeFrame = 0;
            break;
        }
    }
}

void updateBombs() {
    for (auto& b : bombs) {
        if (b.exploding && frameCount > b.explodeFrame + 1)
            b.exploding = false;
    }

    if (frameCount % 3 == 0) {
        for (auto& b : bombs) {
            if (!b.active) continue;
            b.y += b.direction;
            b.x += b.drift;
            if (b.y >= HEIGHT || b.x <= 0 || b.x >= WIDTH) {
                b.active = false;
                continue;
            }
            b.Side_Bombs[0] = {b.x - 1, b.y};
            b.Side_Bombs[1] = {b.x + 1, b.y};
            b.Side_Bombs[2] = {b.x, b.y - 1};
        }
    }

    for (auto& b : bombs) {
        for (auto& bl : bullets) {
            if (!b.active || !bl.active) continue;
            bool destroyed = false;
            for (int i = 0; i < 3; i++) {
                if (b.Side_Bombs[i].x == bl.x && b.Side_Bombs[i].y == bl.y) {
                    playBombDestroyedSound();
                    b.exploding = true;
                    b.explodeFrame = frameCount;
                    b.active = false;
                    bl.active = false;
                    destroyed = true;
                }
            }
            if (!destroyed && b.x == bl.x && b.y == bl.y) {
                playBombDestroyedSound();
                b.exploding = true;
                b.explodeFrame = frameCount;
                b.active = false;
                bl.active = false;
                destroyed = true;
            }

            if (destroyed) {
                if (bl.owner == 'A') playerAScore++;
                else if (bl.owner == 'B') playerBScore++;
            }
        }

        if (b.active) {
            auto hitPlayer = [&](int px, int py, bool& aliveFlag) {
                if (b.x == px && b.y == py)
                    aliveFlag = false;
                for (int i = 0; i < 3; i++)
                    if (b.Side_Bombs[i].x == px && b.Side_Bombs[i].y == py)
                        aliveFlag = false;
                if (!aliveFlag) {
                    b.exploding = true;
                    b.explodeFrame = frameCount;
                    b.active = false;
                    playPlayerHitSound();
                }
            };
            hitPlayer(playerAX, playerAY, Aalive);
            hitPlayer(playerBX, playerBY, Balive);
        }
    }

    if (rand() % 5 == 0) spawnBomb();
}

// ======================================
// 🔫 BULLET LOGIC
// ======================================
void updateBullets() {
    for (auto& b : bullets) {
        if (b.active) {
            b.y += b.direction;
            if (b.y < 0 || b.y >= HEIGHT) b.active = false;
        }
    }
}

// ======================================
// 🎮 INPUT HANDLING
// ======================================
void handleInput() {
    if (Aalive) {
        if (GetAsyncKeyState('A') & 0x8000 && playerAX > 1) playerAX--;
        if (GetAsyncKeyState('D') & 0x8000 && playerAX < WIDTH - 2) playerAX++;
        if (GetAsyncKeyState('W') & 0x8000 && playerAY > 0) playerAY--;
        if (GetAsyncKeyState('S') & 0x8000 && playerAY < HEIGHT - 1) playerAY++;
        if (GetAsyncKeyState('F') & 0x8000) {
            playShootSound();
            for (auto& b : bullets)
                if (!b.active) { b = {playerAX, playerAY - 1, -1, true, 'A'}; break; }
        }
    }

    if (Balive) {
        if (GetAsyncKeyState('J') & 0x8000 && playerBX > 1) playerBX--;
        if (GetAsyncKeyState('L') & 0x8000 && playerBX < WIDTH - 2) playerBX++;
        if (GetAsyncKeyState('I') & 0x8000 && playerBY > 0) playerBY--;
        if (GetAsyncKeyState('K') & 0x8000 && playerBY < HEIGHT - 1) playerBY++;
        if (GetAsyncKeyState('H') & 0x8000) {
            playShootSound();
            for (auto& b : bullets)
                if (!b.active) { b = {playerBX, playerBY - 1, -1, true, 'B'}; break; }
        }
    }
}

// ======================================
// ✅ Game Over Condition Fix
// ======================================
bool checkGameOver() {
    return (!Aalive && !Balive) || gameTimeOver;
}
// ======================================
// 🖥️ DRAW SCREEN + UI
// ======================================
void draw() {
    cout << "\033[1;1H";
    for (int i = 0; i <= WIDTH; i++) cout << COLOR_CYAN << "_" << COLOR_RESET;

    for (int y = 0; y < HEIGHT; y++) {
        cout << "\n|";
        for (int x = 0; x < WIDTH; x++) {
            bool drawn = false;

            for (auto& b : bombs) {
                if (b.exploding && frameCount == b.explodeFrame + 1) {
                    if (x == b.x && y == b.y) {
                        cout << b.color << EXPLOSION_CHAR << COLOR_RESET;
                        drawn = true;
                    }
                    for (int i = 0; i < 3; i++) {
                        if (x == b.Side_Bombs[i].x && y == b.Side_Bombs[i].y) {
                            cout << b.color << EXPLOSION_CHAR << COLOR_RESET;
                            drawn = true;
                        }
                    }
                } else if (b.active) {
                    for (int i = 0; i < 3; i++) {
                        if (x == b.Side_Bombs[i].x && y == b.Side_Bombs[i].y) {
                            cout << b.color << BOMB_CHAR << COLOR_RESET;
                            drawn = true;
                            break;
                        }
                    }
                    if (!drawn && x == b.x && y == b.y) {
                        cout << b.color << BOMB_CHAR << COLOR_RESET;
                        drawn = true;
                    }
                }
                if (drawn) break;
            }

            if (!drawn) {
                for (auto& bl : bullets)
                    if (bl.active && bl.x == x && bl.y == y) {
                        cout << COLOR_CYAN << BULLET_CHAR << COLOR_RESET;
                        drawn = true;
                        break;
                    }
            }

            if (drawn) continue;

            if (x == playerAX && y == playerAY && Aalive)
                cout << COLOR_GREEN << PLAYER_A_CHAR << COLOR_RESET;
            else if (x == playerBX && y == playerBY && Balive)
                cout << COLOR_MAGENTA << PLAYER_B_CHAR << COLOR_RESET;
            else
                cout << " ";
        }
        cout << "|";
    }

    cout << "\n";
    for (int i = 0; i <= WIDTH; i++) cout << "-";

    int elapsed = duration_cast<seconds>(steady_clock::now() - startTime).count();
    int remaining = max(0, gameDurationSec - elapsed);
    if (remaining == 0) gameTimeOver = true;

    cout << "\n" << COLOR_YELLOW << "⏳ Time Left: " << remaining << "s"
         << "  " << COLOR_GREEN << "A: " << playerAScore
         << "  " << COLOR_MAGENTA << "B: " << playerBScore << COLOR_RESET;
}

// ======================================
// 🚀 MAIN FUNCTION
// ======================================
int main() {
    srand(time(0));
    hideCursor();
    cout << "\033[2J\033[1;1H";

    cout << "Enter game duration in minutes: ";
    int minutes;
    cin >> minutes;
    gameDurationSec = minutes * 60;
    startTime = steady_clock::now();

    while (!checkGameOver()) {
        handleInput();
        updateBullets();
        updateBombs();
        draw();

        prevAX = playerAX; prevAY = playerAY;
        prevBX = playerBX; prevBY = playerBY;

        Sleep(50);  // Adjust for smoother performance
        frameCount++;
    }

    playGameOverSound();
    cout << "\n\n" << COLOR_RED << "🎮 Game Over!\n" << COLOR_RESET;

    if (Aalive && !Balive)
        cout << COLOR_GREEN << "🏆 Player A Wins!\n";
    else if (Balive && !Aalive)
        cout << COLOR_MAGENTA << "🏆 Player B Wins!\n";
    else if (playerAScore > playerBScore)
        cout << COLOR_GREEN << "🏆 Player A Wins by Score!\n";
    else if (playerBScore > playerAScore)
        cout << COLOR_MAGENTA << "🏆 Player B Wins by Score!\n";
    else
        cout << COLOR_CYAN << "🤝 It's a Tie!\n";

    cout << COLOR_RESET;
    return 0;
}
