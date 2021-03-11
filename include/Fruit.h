#ifndef FRUIT_H
#define FRUIT_H

#include "tile.h"
#include "SDL.h"

class Fruit : public tile{
public:
    Fruit();
    virtual ~Fruit();
    void initFruit();
    void draw(SDL_Renderer** renderer);
    int getX();
    int getY();
private:
    SDL_Rect rect;
    point fruit;
};

#endif // FRUIT_H
