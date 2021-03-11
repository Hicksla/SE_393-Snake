#include <string>
#include "text.h"
#include "SDL.h"

text::text(){
    line tempLine;
    std::vector<line> tempVect;
    for(int i=0; i<26; i++){
        switch(i){
        case 0:
            tempLine.x1 = 0;
            tempLine.y1 = 1.5;
            tempLine.x2 = 0.5;
            tempLine.y2 = 0;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.5;
            tempLine.y1 = 0;
            tempLine.x2 = 1;
            tempLine.y2 = 1.5;
            tempVect.push_back(tempLine);


            tempLine.x1 = 0.25;
            tempLine.y1 = 0.75;
            tempLine.x2 = 0.75;
            tempLine.y2 = 0.75;
            tempVect.push_back(tempLine);

            Letters.push_back(tempVect);
            tempVect.clear();
            break;
        case 1:
            tempLine.x1 = 0;
            tempLine.y1 = 0;
            tempLine.x2 = 0;
            tempLine.y2 = 1.5;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0;
            tempLine.y1 = 0;
            tempLine.x2 = 0.6;
            tempLine.y2 = 0;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.6;
            tempLine.y1 = 0;
            tempLine.x2 = 0.75;
            tempLine.y2 = 0.15;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0;
            tempLine.y1 = 0.75;
            tempLine.x2 = 0.6;
            tempLine.y2 = 0.75;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.6;
            tempLine.y1 = 0.75;
            tempLine.x2 = 0.75;
            tempLine.y2 = 0.6;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.6;
            tempLine.y1 = 0.75;
            tempLine.x2 = 0.75;
            tempLine.y2 = 0.9;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0;
            tempLine.y1 = 1.5;
            tempLine.x2 = 0.6;
            tempLine.y2 = 1.5;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.6;
            tempLine.y1 = 1.5;
            tempLine.x2 = 0.75;
            tempLine.y2 = 1.35;
            tempVect.push_back(tempLine);


            tempLine.x1 = 0.75;
            tempLine.y1 = 0.15;
            tempLine.x2 = 0.75;
            tempLine.y2 = 0.6;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.75;
            tempLine.y1 = 0.9;
            tempLine.x2 = 0.75;
            tempLine.y2 = 1.35;
            tempVect.push_back(tempLine);

            Letters.push_back(tempVect);
            tempVect.clear();

        case 2:
            tempLine.x1 = 1;
            tempLine.y1 = 0;
            tempLine.x2 = 0;
            tempLine.y2 = 0;
            tempVect.push_back(tempLine);


            tempLine.x1 = 0;
            tempLine.y1 = 0;
            tempLine.x2 = 0;
            tempLine.y2 = 1.5;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0;
            tempLine.y1 = 1.5;
            tempLine.x2 = 1;
            tempLine.y2 = 1.5;
            tempVect.push_back(tempLine);

            Letters.push_back(tempVect);
            tempVect.clear();

        case 3:
            tempLine.x1 = 0.75;
            tempLine.y1 = 0;
            tempLine.x2 = 0;
            tempLine.y2 = 0;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.75;
            tempLine.y1 = 0;
            tempLine.x2 = 0.9;
            tempLine.y2 = 0.15;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0;
            tempLine.y1 = 1.5;
            tempLine.x2 = 0;
            tempLine.y2 = 0;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.75;
            tempLine.y1 = 1.5;
            tempLine.x2 = 0;
            tempLine.y2 = 1.5;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.9;
            tempLine.y1 = 0.15;
            tempLine.x2 = 0.9;
            tempLine.y2 = 1.35;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0.75;
            tempLine.y1 = 1.5;
            tempLine.x2 = 0.9;
            tempLine.y2 = 1.35;
            tempVect.push_back(tempLine);

            Letters.push_back(tempVect);
            tempVect.clear();

        case 4:
            tempLine.x1 = 0;
            tempLine.y1 = 0;
            tempLine.x2 = 0;
            tempLine.y2 = 1.5;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0;
            tempLine.y1 = 0;
            tempLine.x2 = 1;
            tempLine.y2 = 0;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0;
            tempLine.y1 = 0.6;
            tempLine.x2 = 0.75;
            tempLine.y2 = 0.6;
            tempVect.push_back(tempLine);

            tempLine.x1 = 0;
            tempLine.y1 = 1.5;
            tempLine.x2 = 1;
            tempLine.y2 = 1.5;
            tempVect.push_back(tempLine);

            Letters.push_back(tempVect);
            tempVect.clear();

        }
    }
}

text::~text(){

}


void text::draw(SDL_Renderer** renderer, const char* input, int x, int y, int fontSize){/*
    for(int i=0; i<strlen(&input); i++){
        switch(&input[i]){
        case "a":
            for(int j=0; j<Letters.at(0).size(); j++){
                int x1 = x + (Letters.at(0).at(j).x1*fontSize);
                int y1 = y + (Letters.at(0).at(j).y1*fontSize);
                int x2 = x + (Letters.at(0).at(j).x2*fontSize);
                int y2 = y + (Letters.at(0).at(j).y2*fontSize);
                SDL_RenderDrawLine(*renderer, x1, y1, x2, y2);
            }
            x += fontSize + 10;*/
            for(int j=0; j<Letters.at(4).size(); j++){
                int x1 = x + (Letters.at(4).at(j).x1*fontSize);
                int y1 = y + (Letters.at(4).at(j).y1*fontSize);
                int x2 = x + (Letters.at(4).at(j).x2*fontSize);
                int y2 = y + (Letters.at(4).at(j).y2*fontSize);
                SDL_RenderDrawLine(*renderer, x1, y1, x2, y2);
            }
        //}
    //}
    return;
}


