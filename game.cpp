#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

const int WIDTH = 30, HEIGHT = 30, MAX_BULLETS = 34, MAX_BOMBS = 4;
bool Aalive = true, Balive = true;

struct Bullet {
    int x, y, direction;
    bool active = false;
} bullets[MAX_BULLETS];

struct Bomb {
    int x, y, direction;
    bool active = false;
    int direction2;
} bombs[MAX_BOMBS];

int playerAX = WIDTH / 4, playerAY = HEIGHT - 1, prevAX = playerAX, prevAY = playerAY;
int playerBX = (WIDTH * 3) / 4, playerBY = HEIGHT - 1, prevBX = playerBX, prevBY = playerBY;
int frameCount = 0;

// Colors
string colorGreen = "\033[1;32m";
string colorMagenta = "\033[1;35m";
string colorRed = "\033[1;31m";
string colorBlue = "\033[1;34m";
string colorCyan = "\033[1;36m";
string colorReset = "\033[0m";

void hideCursor() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    SetConsoleCursorInfo(handle, &info);
}

void updateBullets() {
    for (auto& bullet : bullets) {
        if (bullet.active) {
            bullet.y += bullet.direction;
            if (bullet.y < 0 || bullet.y >= HEIGHT)
                bullet.active = false;
        }
    }
}

void callbomb() {
    for (auto& bomb : bombs) {
        if (!bomb.active) {
            bomb = {
                rand() % (WIDTH - 2) + 1,
                0,
                1,
                true,
                rand() % 3 - 1 // -1, 0, or 1
            };
        }
    }
}

void checkBombPlayerCollision(Bomb& bomb) {
    if (!bomb.active) return;

    if (bomb.x == playerAX && bomb.y == playerAY) {
        bomb.active = false;
        Aalive = false;
    }
    if (bomb.x == playerBX && bomb.y == playerBY) {
        bomb.active = false;
        Balive = false;
    }
}

void updatebomb() {
    if (frameCount % 3 == 0) {
        for (auto& bomb : bombs) {
            if (bomb.active) {
                bomb.y += bomb.direction;
                bomb.x += bomb.direction2;
                if (bomb.y < 0 || bomb.y >= HEIGHT || bomb.x < 0 || bomb.x >= WIDTH)
                    bomb.active = false;
            }
        }
    }

    for (auto& bullet : bullets) {
        for (auto& bomb : bombs) {
            if (bullet.active && bomb.active && bullet.x == bomb.x && bullet.y == bomb.y) {
                bullet.active = false;
                bomb.active = false;
            }
        }
    }

    for (auto& bomb : bombs) {
        checkBombPlayerCollision(bomb);
    }
}

void draw() {
    cout << "\033[1;1H";
    for (int i = 0; i <= WIDTH; i++) cout << colorCyan << "_" << colorReset;

    for (int y = 0; y < HEIGHT; y++) {
        cout << "\n|";
        for (int x = 0; x < WIDTH; x++) {
            bool drawn = false;

            for (auto& bomb : bombs) {
                if (bomb.active && bomb.x == x && bomb.y == y) {
                    cout << colorRed << "O" << colorReset;
                    drawn = true;
                    break;
                }
            }
            if (drawn) continue;

            for (auto& bullet : bullets) {
                if (bullet.active && bullet.x == x && bullet.y == y) {
                    cout << colorCyan << "|" << colorReset;
                    drawn = true;
                    break;
                }
            }
            if (drawn) continue;

            if (x == playerAX && y == playerAY && x == playerBX && y == playerBY)
                cout << ((frameCount % 4 == 0) ? colorRed : (frameCount % 2 == 0) ? colorGreen : colorBlue) << '@' << colorReset;
            else if (x == playerAX && y == playerAY && Aalive)
                cout << ((frameCount % 2 == 0) ? colorGreen : colorRed) << 'A' << colorReset;
            else if (x == playerBX && y == playerBY && Balive)
                cout << ((frameCount % 2 == 0) ? colorMagenta : colorBlue) << 'B' << colorReset;
            else if ((x == prevAX && y == prevAY) || (x == prevBX && y == prevBY))
                cout << colorCyan << '.' << colorReset;
            else
                cout << " ";
        }
        cout << "|";
    }

    cout << "\n";
    for (int i = 0; i <= WIDTH; i++) {
        int half = WIDTH / 2;
        if (i < half)
            cout << colorMagenta << "-" << colorReset;
        else
            cout << colorCyan << "-" << colorReset;
    }
}

void handleInput() {
    if (_kbhit()) {
        char key = _getch();

        if (key == 'f' && Aalive) {
            for (auto& bullet : bullets) {
                if (!bullet.active) {
                    bullet = {playerAX, playerAY - 1, -1, true};
                    break;
                }
            }
        }

        if (key == 'j' && Balive) {
            for (auto& bullet : bullets) {
                if (!bullet.active) {
                    bullet = {playerBX, playerBY - 1, -1, true};
                    break;
                }
            }
        }

        if (Aalive) {
            if (key == 'a' && playerAX > 1) playerAX -= 1;
            else if (key == 'd' && playerAX < WIDTH - 2) playerAX += 1;
            else if (key == 'w' && playerAY > 0) playerAY--;
            else if (key == 's' && playerAY < HEIGHT - 1) playerAY++;
        }

        if (Balive) {
            if (key == 'k' && playerBX > 1) playerBX -= 1;
            else if (key == ';' && playerBX < WIDTH - 2) playerBX += 1;
            else if (key == 'o' && playerBY > 0) playerBY--;
            else if (key == 'l' && playerBY < HEIGHT - 1) playerBY++;
        }

        while (_kbhit()) _getch(); // Clear input buffer
    }
}

int main() {
    srand(time(0));
    hideCursor();
    cout << "\033[2J\033[1;1H"; // Clear screen

    while (Aalive && Balive) {
        handleInput();
        callbomb();
        updatebomb();
        updateBullets();
        draw();

        prevAX = playerAX; prevAY = playerAY;
        prevBX = playerBX; prevBY = playerBY;

        Sleep(17);
        frameCount++;
    }

    cout << "\nGame Over\n";
    if (Aalive)
        cout << "Player A is the winner!\n";
    else
        cout << "Player B is the winner!\n";

    return 0;
}