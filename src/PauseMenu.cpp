#include <math.h>
#include "PauseMenu.h"
#include "SDL.h"
#include "SDL_image.h"
#include "fatalError.h"

PauseMenu::PauseMenu(){
    background.w = 300;
    background.h = 200;
    background.x = (1280-background.w)/2;
    background.y = (720-background.h)/2;

    imageSurface = nullptr;
}

PauseMenu::~PauseMenu(){

}

void PauseMenu::initialize(SDL_Window* window){

    return;
}

void PauseMenu::drawCircle(SDL_Renderer** renderer, int radius){
    const double pi = 3.141592653589793238264;
    for(double i=0; i<2*pi; i += 0.1){
        SDL_RenderDrawPoint(*renderer, 300+(radius*cos(i)), 500+(radius*sin(i)));
    }

    return;
}

void PauseMenu::draw(SDL_Renderer** renderer){
    int fontSize = 30;
    int x = 100;
    int y = 100;
    SDL_SetRenderDrawColor(*renderer, 0, 0, 255, 225);
    SDL_RenderFillRect(*renderer, &background);

    SDL_RenderPresent(*renderer);
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_RenderClear(*renderer);


    return;
}
