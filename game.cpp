#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

const int width = 61;
const int height = 30;
//bullet
const int mexb=20;
struct bull{
int x,y;
int x2,y2;
bool active=false;
bool active2=false;
};
bull n[mexb];

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
// upgrade bullet
void bullup(){
    for(int k=0;k<mexb;k++){
    if(n[k].active){
        n[k].y--;
        if(n[k].y<0){
            n[k].active=false;
        }
    }


    }
     for(int k=0;k<mexb;k++){
    if(n[k].active2){
        n[k].y2--;
        if(n[k].y2<0){
            n[k].active2=false;
        }
    }


    }


}
// Draw the arena
void draw() {
    std::cout << "\033[1;1H"; // Move cursor to top-left

    // Top border
    for (int i = 0; i <= width; i++) cout << cyan << "_" << reset;

    for (int y = 0; y < height; y++) {
        cout << "\n|";
        for (int x = 0; x < width; x++) {
            //bullet 
             bool draw=false;
            for(int j=0;j<mexb;j++){
                if(n[j].active){
                if(x==n[j].x && y==n[j].y){
                 cout<<"|";
                  draw=true;
                   break;
                  

                }

            }

            }
            if( draw){
                continue;
            }
            //bullet 2
             draw=false;
            for(int j=0;j<mexb;j++){
                if(n[j].active2){
                if(x==n[j].x2 && y==n[j].y2){
                 cout<<"|";
                  draw=true;
                   break;
                  

                }

            }

            }
            if( draw){
                continue;
            }

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
                else cout <<  red << PLAYER_A_CHAR;
                continue;
            }

            // Player B
            if (x == playerBX && y == playerBY) {
                if (fc % 2 == 0) cout << magenta << PLAYER_B_CHAR << reset;
                else cout << blue << PLAYER_B_CHAR ;
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
           // bullet A (f)
           if(ch=='f'){
               for(int i=0;i<mexb;i++){
                if(!n[i].active){
                    n[i].active=true;
                    n[i].x=playerAX;
                     n[i].y=playerAY-1;
                     break;
                }
               }
           }
           //bullet B (j)
            if(ch=='j'){
               for(int i=0;i<mexb;i++){
                if(!n[i].active2){
                    n[i].active2=true;
                    n[i].x2=playerBX;
                     n[i].y2=playerBY-1;
                     break;
                }
               }
           }
        // Player A (WASD)
        if (ch == 'a' && playerAX > 1) playerAX -= 2;
        else if (ch == 'd' && playerAX < width - 2) playerAX += 2;
        else if (ch == 'w' && playerAY > 0) playerAY--;
        else if (ch == 's' && playerAY < height - 1) playerAY++;

        // Player B (IJKL)
        if (ch == 'k' && playerBX > 1) playerBX -= 2;
        else if (ch == ';' && playerBX < width - 2) playerBX += 2;
        else if (ch == 'o' && playerBY > 0) playerBY--;
        else if (ch == 'l' && playerBY < height - 1) playerBY++;

        while (_kbhit()) _getch(); // Clear buffer
    }
}

int main() {
    hideCursor();
    std::cout << "\033[2J\033[1;1H"; // Clear screen

    while (true) {
        input();
         bullup();
        draw();

        
        // Update previous positions
        prevAX = playerAX; prevAY = playerAY;
        prevBX = playerBX; prevBY = playerBY;

        Sleep(7);
        fc++;
    }

    return 0;
}