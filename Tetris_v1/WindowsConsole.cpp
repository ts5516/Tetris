#include "Game.h"

using namespace std;

int main()
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