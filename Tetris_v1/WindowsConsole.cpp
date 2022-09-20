#include "Screen.h"
#include "Game.h"
#include "Input.h"

using namespace std;

void gameStart(const int& speed)
{
    Game game;
    Input input;
    KEYCODE key;
    
    while (true)
    {
        key = input.getInput();

        game.gameUpdate(key);
       
        game.gameRender();
    }
}

int main()
{
    gameStart(1);
}