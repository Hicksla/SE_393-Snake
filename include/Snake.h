#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "tile.h"
#include "SDL.h"

class SnakeClass : public tile{
public:
    SnakeClass();
    virtual ~SnakeClass();
    void draw(SDL_Renderer** renderer);
    void initSnake();
    void updatePos(Uint32 time);
    void setDirection(char dir);
    void die();
    bool getDeadness();
    int getScore();


    enum class direction{
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    direction snakeDir;
    SDL_Rect fruitRect;
    Uint32 timeFacingWall;
private:
    bool checkBounds(int mouseX, int mouseY, int x, int y, int width, int height);
    void ateFruit();

    int score;

    bool isDead;
    point fruit;
    std::vector<point> snake;
};

#endif // SNAKE_H
