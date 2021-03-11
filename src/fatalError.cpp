#include <iostream>
#include "fatalError.h"
#include "SDL.h"

void fatalError(std::string err){
    std::cout << err << std::endl;
	SDL_Quit();
}

