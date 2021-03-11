#include <iostream>
#include "SDL.h"
#include "MainGame.h"
#undef main

int main(int arc, char* argv[]){
    MainGame game;
    game.run();
    return 0;
}
