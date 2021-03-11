#ifndef TILE_H
#define TILE_H


class tile{
public:
    tile();
    virtual ~tile();
    void setTileWidth(int newWidth);
    void setTileHeight(int newHeight);
    void initTile();

    int width;
    int height;
    struct point{
        int x;
        int y;
    };
    struct color{
        int r;
        int g;
        int b;
    };
private:
};

#endif // TILE_H
