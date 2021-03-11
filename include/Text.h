#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>
#include "SDL.h"

class text{
public:
    text();
    ~text();

    void draw(SDL_Renderer** renderer, const char* input, int x, int y, int fontSize);
private:
    struct line {
        float x1;
        float y1;
        float x2;
        float y2;
    };
    std::vector< std::vector<line> > Letters;
};

#endif // TEXT_H
