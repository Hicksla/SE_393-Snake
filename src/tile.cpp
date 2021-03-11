#include <iostream>
#include "tile.h"

tile::tile(){
    width = 20;
    height = 20;

}

tile::~tile(){

}

void tile::initTile(){
    width = 20;
    height = 20;
}

void tile::setTileWidth(int newWidth){
    width = newWidth/64;
}
void tile::setTileHeight(int newHeight){
    height = newHeight/36;
}
