#ifndef MAINGAME_H
#define MAINGAME_H

#include <string>
#include "SDL.h"
#include "Snake.h"
#include "PauseMenu.h"
//#include "Text.h"

class MainGame{
public:
    MainGame();
    virtual ~MainGame();
    void run();
    void draw();
//    void updateTitleScore(int score);

    SDL_Surface* surface;
private:
    void initialize();
    void loop();
    void processInput();


    int ScreenWidth;
    int ScreenHeight;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    SDL_Rect borderRect;
    SDL_Rect PausedMenu;

    enum class state{
        PLAY,
        EXIT,
        DIED
    };
    state gameState;

    bool paused;

    SnakeClass snake;
    SnakeClass opponent;
    tile tileObj;
    PauseMenu pause;
//    Text textObj;
};

std::string int_to_str(int x);

#endif // MAINGAME_H
