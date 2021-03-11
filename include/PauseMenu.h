#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "SDL.h"
#include "text.h"

class PauseMenu{
public:
    PauseMenu();
    ~PauseMenu();

    void initialize(SDL_Window* window);
    void draw(SDL_Renderer** renderer);
private:
    void drawCircle(SDL_Renderer** renderer, int radius);

    SDL_Surface* imageSurface;

    SDL_Rect textRect;
    SDL_Rect background;

    text textClass;
};

#endif // PAUSEMENU_H
