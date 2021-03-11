#include <ctime>
#include <random>
#include <iostream>
#include "Fruit.h"

Fruit::Fruit(){

}

Fruit::~Fruit(){

}

void Fruit::initFruit(){
    srand(time(0));
    fruit.x = 1+(rand()%62);
    fruit.y = 1+(rand()%34);
    std::cout << fruit.x << "." << fruit.y << std::endl;
}

void Fruit::draw(SDL_Renderer** renderer){
    rect.x = width*fruit.x;
    rect.y = height*fruit.y;
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(*renderer, &rect);
}

int Fruit::getX(){
    std::cout << fruit.x %62 << std::endl;
    return fruit.x%62;
}

int Fruit::getY(){
    return fruit.y%62;
}
