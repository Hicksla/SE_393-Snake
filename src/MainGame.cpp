#include <iostream>
#include <string>
#include <sstream>
#include "fatalError.h"
#include "MainGame.h"
#include "Snake.h"
#include "tile.h"
#include "PauseMenu.h"
#include "fatalError.h"

MainGame::MainGame(){
    window = nullptr;
    renderer = nullptr;
    surface = nullptr;
    ScreenHeight = 720;
    ScreenWidth = 1280;
    gameState = state::PLAY;

    paused = false;
    PausedMenu.h = 200;
    PausedMenu.w = 300;
    PausedMenu.x = (ScreenWidth-PausedMenu.w)/2;
    PausedMenu.y = (ScreenHeight-PausedMenu.h)/2;


    initialize();
}

MainGame::~MainGame(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void MainGame::initialize(){
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, 0);
    if(window==nullptr){
        fatalError("Error creating the window");
        SDL_Quit();
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer==nullptr){
        fatalError("Error Creating the renderer");
    }

    snake.initSnake();
    tileObj.initTile();
    pause.initialize(window);
}

void MainGame::run(){
    loop();
}

void MainGame::loop(){
    unsigned int lastTime = 0, currentTime;
    while(gameState!=state::EXIT){
        processInput();
        currentTime = SDL_GetTicks();
        if(gameState==state::DIED){
            pause.draw(&renderer);
        }
        else if(gameState==state::PLAY){
            if(!paused){
                if(currentTime>=lastTime+100){
                    draw();
    //                updateTitleScore(snake.getScore());
                    lastTime = currentTime;
                }
                if(snake.getDeadness()){
                    gameState = state::DIED;
                }
            }
            else{
                pause.draw(&renderer);
            }
        }
    }
}


void MainGame::draw(){
    borderRect.x = 0;
    borderRect.y = 0;
    borderRect.w = 1280;
    borderRect.h = 720;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &borderRect);

    borderRect.x = 20;
    borderRect.y = 20;
    borderRect.w = 1240;
    borderRect.h = 680;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &borderRect);

    snake.updatePos(SDL_GetTicks());
    snake.draw(&renderer);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void MainGame::processInput(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
            gameState = state::EXIT;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode){
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                snake.setDirection('u');
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                snake.setDirection('l');
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                snake.setDirection('d');
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                snake.setDirection('r');
                break;
            case SDL_SCANCODE_SPACE:
                if(gameState==state::DIED){
                    snake.initSnake();
                    gameState=state::PLAY;
                }
                break;
            case SDL_SCANCODE_P:
                paused = !paused;
            default:
                break;
            }
            break;
        }
    }
}
