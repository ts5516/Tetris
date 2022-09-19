#include <vector>
#include <conio.h>
#include "Screen.h"
#include "Game.h"
#include <iostream>
using namespace std;

int keyInput()
{
    if (_kbhit())
    {
        int ch = _getch();
        if (ch == 32) // space is pause;
            return 0;
        else if (ch == 224)
        {
            return _getch(); // up: 72, down : 80 left: 75 right: 77;
        }
        else if (ch == 97 || ch == 115 || ch == 100 || ch == 119 || ch == 120) //asdwx
        {
            return ch;
        }
    }

    return -1;
}

void gameStart(const int& speed)
{
    Game game;

    int input = 1;
    
    while (true)
    {
        input = keyInput();

        game.gameUpdate(input);
       
        game.gameRender();
    }
}

int main()
{
    gameStart(1);
}