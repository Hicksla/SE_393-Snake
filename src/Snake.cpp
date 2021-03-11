#include <iostream>
#include <ctime>
#include <random>
#include "Snake.h"
//#include "fatalError.h"
#include "SDL.h"

SnakeClass::SnakeClass(){
    isDead = false;
    snakeDir = direction::DOWN;
    timeFacingWall = 0x00000000;
    score = 0;
}

SnakeClass::~SnakeClass(){
    snake.clear();
}

void SnakeClass::initSnake(){
    isDead = false;
    point tempPoint;
    tempPoint = {1, 1};
    if(snake.size()>0){
        snake.clear();
    }
    snake.push_back(tempPoint);
    srand(time(0));
    fruit.x = 1+(rand()%62);
    fruit.y = 1+(rand()%34);


}

void SnakeClass::updatePos(Uint32 time){
    if(fruit.x==snake.at(0).x && fruit.y==snake.at(0).y){
        ateFruit();
    }
    if(snake.size()>=4){
        for(unsigned int i=3; i<snake.size(); i++){
            switch(snakeDir){
            case direction::LEFT:
                if((snake.at(0).x-1==snake.at(i).x && snake.at(0).y==snake.at(i).y)){
                    die();
                }
                break;
            case direction::UP:
                if((snake.at(0).x==snake.at(i).x && snake.at(0).y-1==snake.at(i).y)){
                    die();
                }
                break;
            case direction::RIGHT:
                if((snake.at(0).x+1==snake.at(i).x && snake.at(0).y==snake.at(i).y)){
                    die();
                }
                break;
            case direction::DOWN:
                if((snake.at(0).x==snake.at(i).x && snake.at(0).y+1==snake.at(i).y)){
                    die();
                }
                break;
            }
        }
    }
    switch(snakeDir){
    case direction::LEFT:
        if(checkBounds((snake.at(0).x+62)*width, snake.at(0).y*height, 0, 0, 1280, 720)){
            if(time>=timeFacingWall+0xC8){
                die();
            }
        }
        else{
            for(unsigned int i=snake.size()-1; i>0; i--){
                snake.at(i) = snake.at(i-1);
            }
            snake.at(0).x--;
            timeFacingWall+=time;
        }
        break;
    case direction::UP:
        if(checkBounds(snake.at(0).x*width, (snake.at(0).y+34)*height, 0, 0, 1280, 720)){
            if(time>=timeFacingWall+150){
                die();
            }
        }
        else{
            for(unsigned int i=snake.size()-1; i>0; i--){
                snake.at(i) = snake.at(i-1);
            }
            snake.at(0).y--;
            timeFacingWall+=time;
        }
        break;
    case direction::RIGHT:
        if(checkBounds((snake.at(0).x-61)*width, snake.at(0).y*height, 0, 0, 1280, 720)){
            if(time>=timeFacingWall+150){
                die();
            }
        }
        else{
            for(unsigned int i=snake.size()-1; i>0; i--){
                snake.at(i) = snake.at(i-1);
            }
            snake.at(0).x++;
            timeFacingWall+=time;
        }
        break;
    case direction::DOWN:
        if(checkBounds(snake.at(0).x*width, (snake.at(0).y-34)*height, 0, 0, 1280, 720)){
            if(time>=timeFacingWall+150){
                die();
            }
        }
        else{
            for(unsigned int i=snake.size()-1; i>0; i--){
                snake.at(i) = snake.at(i-1);
            }
            snake.at(0).y++;
            timeFacingWall+=time;
        }
        break;
    }
}

void SnakeClass::ateFruit(){
    point tempPoint;
    score += 100;
    bool isFruitPlaceable = false;
    if(snake.size()==1){
        switch(snakeDir){
        case direction::LEFT:
            tempPoint = {snake.at(0).x+1, snake.at(0).y};
            break;
        case direction::UP:
            tempPoint = {snake.at(0).x, snake.at(0).y+1};
            break;
        case direction::RIGHT:
            tempPoint = {snake.at(0).x-1, snake.at(0).y};
            break;
        case direction::DOWN:
            tempPoint = {snake.at(0).x, snake.at(0).y-1};
            break;
        }
    }
    else{
        if(snake.at(snake.size()-1).x==snake.at(snake.size()-2).x){
            tempPoint = {snake.at(snake.size()-1).x, snake.at(snake.size()-1).y-(snake.at(snake.size()-1).y-snake.at(snake.size()-1).y)};
        }
        else{
            tempPoint = {snake.at(snake.size()-1).x-(snake.at(snake.size()-1).x-snake.at(snake.size()-1).x), snake.at(snake.size()-1).y};
        }
    }
    snake.push_back(tempPoint);
    while(!isFruitPlaceable){
        isFruitPlaceable = true;
        fruit.x = 1+(rand()%62);
        fruit.y = 1+(rand()%34);
        for(unsigned int i=0; i<snake.size(); i++){
            if(fruit.x==snake.at(i).x && fruit.y==snake.at(i).y){
                isFruitPlaceable = false;
            }
        }
    }
}

void SnakeClass::draw(SDL_Renderer **renderer){
    fruitRect.x = width*fruit.x;
    fruitRect.y = height*fruit.y;
    fruitRect.w = width;
    fruitRect.h = height;

    SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(*renderer, &fruitRect);

    for(unsigned int i=0; i<snake.size(); i++){
        fruitRect.x = width*snake.at(i).x;
        fruitRect.y = height*snake.at(i).y;
        fruitRect.w = width;
        fruitRect.h = height;

        SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(*renderer, &fruitRect);
    }
    timeFacingWall=SDL_GetTicks();
}

void SnakeClass::setDirection(char dir){
    switch(dir){
    case 'l':
        if(snake.at(0).x!=1 && ((snake.size()>=2 && snakeDir!=direction::RIGHT) || snake.size()==1)){
            snakeDir = direction::LEFT;
        }
        break;
    case 'u':
        if(snake.at(0).y!=1 && ((snake.size()>=2 && snakeDir!=direction::DOWN) || snake.size()==1)){
            snakeDir = direction::UP;
        }
        break;
    case 'r':
        if(snake.at(0).x!=62 && ((snake.size()>=2 && snakeDir!=direction::LEFT) || snake.size()==1)){
            snakeDir = direction::RIGHT;
        }
        break;
    case 'd':
        if(snake.at(0).y!=34 && ((snake.size()>=2 && snakeDir!=direction::UP) || snake.size()==1)){
            snakeDir = direction::DOWN;
        }
        break;
    }
}

bool SnakeClass::checkBounds(int snakeX, int snakeY, int x, int y, int width, int height){
    if(snakeX>x && snakeX<x+width){
        if(snakeY>=y && snakeY<y+height){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}


bool SnakeClass::getDeadness(){
    return isDead;
}

void SnakeClass::die(){
    isDead = true;
}

int SnakeClass::getScore(){
    return score;
}
