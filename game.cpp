#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

const int width = 61;
const int height = 30;

// Player A positions
int playerAX = width / 4, playerAY = height - 1;
int prevAX = playerAX, prevAY = playerAY;

// Player B positions
int playerBX = (width * 3) / 4, playerBY = height - 1;
int prevBX = playerBX, prevBY = playerBY;

int fc = 0;

// Symbols
const char PLAYER_A_CHAR = 'A';
const char PLAYER_B_CHAR = 'B';
const char COLLIDE_CHAR = '@';
const char TRAIL_CHAR = '.';

// Colors
string green = "\033[1;32m";
string magenta = "\033[1;35m";
string red = "\033[1;31m";
string blue = "\033[1;34m";
string cyan = "\033[1;36m";
string reset = "\033[0m";

// Hide cursor
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Draw the arena
void draw() {
    std::cout << "\033[1;1H"; // Move cursor to top-left

    // Top border
    for (int i = 0; i <= width; i++) cout << cyan << "_" << reset;

    for (int y = 0; y < height; y++) {
        cout << "\n|";
        for (int x = 0; x < width; x++) {
            // Collision
            if (x == playerAX && y == playerAY && x == playerBX && y == playerBY) {
                if (fc % 4 == 0) cout << red << COLLIDE_CHAR;
                else if (fc % 2 == 0) cout << green << COLLIDE_CHAR;
                else if (fc % 5 == 0) cout << blue << COLLIDE_CHAR;
                else cout << " ";
                cout << reset;
                continue;
            }

            // Player A
            if (x == playerAX && y == playerAY) {
                if (fc % 2 == 0) cout << green << PLAYER_A_CHAR << reset;
                else cout << " ";
                continue;
            }

            // Player B
            if (x == playerBX && y == playerBY) {
                if (fc % 2 == 0) cout << magenta << PLAYER_B_CHAR << reset;
                else cout << " ";
                continue;
            }

            // Trails
            if ((x == prevAX && y == prevAY) || (x == prevBX && y == prevBY)) {
                cout << cyan << TRAIL_CHAR << reset;
                continue;
            }

            cout << " ";
        }
        cout << "|";
    }

    // Bottom border
    cout << "\n";
    for (int i = 0; i <= width; i++) cout << magenta << "-" << reset;
}

// Handle input
void input() {
    if (_kbhit()) {
        char ch = _getch();

        // Player A (WASD)
        if (ch == 'a' && playerAX > 1) playerAX -= 2;
        else if (ch == 'd' && playerAX < width - 2) playerAX += 2;
        else if (ch == 'w' && playerAY > 0) playerAY--;
        else if (ch == 's' && playerAY < height - 1) playerAY++;

        // Player B (IJKL)
        if (ch == 'j' && playerBX > 1) playerBX -= 2;
        else if (ch == 'l' && playerBX < width - 2) playerBX += 2;
        else if (ch == 'i' && playerBY > 0) playerBY--;
        else if (ch == 'k' && playerBY < height - 1) playerBY++;

        while (_kbhit()) _getch(); // Clear buffer
    }
}

int main() {
    hideCursor();
    std::cout << "\033[2J\033[1;1H"; // Clear screen

    while (true) {
        input();
        draw();

        // Update previous positions
        prevAX = playerAX; prevAY = playerAY;
        prevBX = playerBX; prevBY = playerBY;

        Sleep(15);
        fc++;
    }

    return 0;
}