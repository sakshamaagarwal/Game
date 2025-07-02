#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

const int WIDTH = 30, HEIGHT = 25, MAX_BULLETS = 34, MAX_BOMBS = 6;
bool Aalive = true, Balive = true;

struct Bullet {
    int x, y, direction;
    bool active = false;
} bullets[MAX_BULLETS];

struct Bomb {
    int x, y, direction, drift;
    bool active = false;
} bombs[MAX_BOMBS];

int playerAX = WIDTH / 4, playerAY = HEIGHT - 1, prevAX = playerAX, prevAY = playerAY;
int playerBX = (WIDTH * 3) / 4, playerBY = HEIGHT - 1, prevBX = playerBX, prevBY = playerBY;
int frameCount = 0;

// Colors
string green = "\033[1;32m", magenta = "\033[1;35m", red = "\033[1;31m";
string blue = "\033[1;34m", cyan = "\033[1;36m", reset = "\033[0m";

void hideCursor() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    SetConsoleCursorInfo(h, &info);
}

void spawnBomb() {
    for (auto& b : bombs) {
        if (!b.active) {
            b = {rand() % (WIDTH - 2) + 1, 0, 1, rand() % 3 - 1, true};
            break;
        }
    }
}

void updateBombs() {
    if (frameCount % 3 == 0) {
        for (auto& b : bombs) {
            if (b.active) {
                b.y += b.direction;
                b.x += b.drift;
                if (b.y >= HEIGHT || b.x <= 0 || b.x >= WIDTH) b.active = false;
            }
        }
    }

    for (auto& b : bombs) {
        for (auto& bl : bullets) {
            if (b.active && bl.active && b.x == bl.x && b.y == bl.y) {
                b.active = false;
                bl.active = false;
            }
        }
        if (b.active && b.x == playerAX && b.y == playerAY) {
            b.active = false;
            Aalive = false;
        }
        if (b.active && b.x == playerBX && b.y == playerBY) {
            b.active = false;
            Balive = false;
        }
    }

    if (rand() % 5 == 0) spawnBomb();
}

void updateBullets() {
    for (auto& b : bullets) {
        if (b.active) {
            b.y += b.direction;
            if (b.y < 0 || b.y >= HEIGHT) b.active = false;
        }
    }
}

void handleInput() {
    if (Aalive) {
        if (GetAsyncKeyState('A') & 0x8000 && playerAX > 1) playerAX--;
        if (GetAsyncKeyState('D') & 0x8000 && playerAX < WIDTH - 2) playerAX++;
        if (GetAsyncKeyState('W') & 0x8000 && playerAY > 0) playerAY--;
        if (GetAsyncKeyState('S') & 0x8000 && playerAY < HEIGHT - 1) playerAY++;
        if (GetAsyncKeyState('F') & 0x8000) {
            for (auto& b : bullets) {
                if (!b.active) {
                    b = {playerAX, playerAY - 1, -1, true};
                    break;
                }
            }
        }
    }

    if (Balive) {
        if (GetAsyncKeyState('J') & 0x8000 && playerBX > 1) playerBX--;
        if (GetAsyncKeyState('L') & 0x8000 && playerBX < WIDTH - 2) playerBX++;
        if (GetAsyncKeyState('I') & 0x8000 && playerBY > 0) playerBY--;
        if (GetAsyncKeyState('K') & 0x8000 && playerBY < HEIGHT - 1) playerBY++;
        if (GetAsyncKeyState('H') & 0x8000) {
            for (auto& b : bullets) {
                if (!b.active) {
                    b = {playerBX, playerBY - 1, -1, true};
                    break;
                }
            }
        }
    }
}

void draw() {
    cout << "\033[1;1H";
    for (int i = 0; i <= WIDTH; i++) cout << cyan << "_" << reset;

    for (int y = 0; y < HEIGHT; y++) {
        cout << "\n|";
        for (int x = 0; x < WIDTH; x++) {
            bool drawn = false;

            for (auto& b : bombs)
                if (b.active && b.x == x && b.y == y) { cout << red << "O" << reset; drawn = true; break; }

            if (!drawn) {
                for (auto& bl : bullets)
                    if (bl.active && bl.x == x && bl.y == y) { cout << cyan << "|" << reset; drawn = true; break; }
            }

            if (drawn) continue;

            if (x == playerAX && y == playerAY && Aalive)
                cout << ((frameCount % 2 == 0) ? green : red) << 'A' << reset;
            else if (x == playerBX && y == playerBY && Balive)
                cout << ((frameCount % 2 == 0) ? magenta : blue) << 'B' << reset;
            else if ((x == prevAX && y == prevAY) || (x == prevBX && y == prevBY))
                cout << cyan << '.' << reset;
            else
                cout << " ";
        }
        cout << "|";
    }

    cout << "\n";
    for (int i = 0; i <= WIDTH; i++)
        cout << ((i < WIDTH / 2) ? magenta : cyan) << "-" << reset;
}

int main() {
    srand(time(0));
    hideCursor();
    cout << "\033[2J\033[1;1H";

    while (Aalive && Balive) {
        handleInput();
        updateBullets();
        updateBombs();
        draw();

        prevAX = playerAX; prevAY = playerAY;
        prevBX = playerBX; prevBY = playerBY;

        Sleep(17);
        frameCount++;
    }

    cout << "\n\n" << red << "Game Over!\n" << reset;
    cout << (Aalive ? green + "Player A Wins!\n" : magenta + "Player B Wins!\n") << reset;
    return 0;
}