#include "Game.h"

using namespace std;

void gameStart(const int& speed)
{
    Game game;
    KEYCODE key;
    
    while (game.getGamestate() != GAMESTATE::END)
    {
        key = game.getInput();
        
        game.gameUpdate(key);
       
        game.gameRender();
    }
}

int main()
{
    gameStart(1);
}