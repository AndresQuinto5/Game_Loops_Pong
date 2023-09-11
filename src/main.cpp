#include <iostream>
#include "Game/game.h"



Game *game = nullptr;

int main() {
    game = new Game();
    game->init("PongGame", 1200, 800);
    game->setup();
    while (game->running())
    {
        game->frameStart();
        game->handleEvents();
        game->update();
        game->render();
        game->frameEnd();
    }

    game->clean();


    return 0;
}