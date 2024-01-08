
#pragma once
#include "gameobject.hpp"
#include "utils/vec.h"
#include "utils/rect.h"

#define TILESIZE 16

class Tile: public GameObject {

private:
    Vector2<double> size_; // tile will be outlined by pos and pos + size
    Rect<double> rect_;

public:
    Tile();
    Tile(Vector2<double> pos, sf::Texture texture);
    ~Tile();

    Rect<double> getRect(void);
};

Tile::Tile(): GameObject() { } // Default
Tile::Tile(Vector2<double> pos, sf::Texture texture): GameObject(pos , texture), size_(Vector2<double>(10,10)), rect_(pos, pos + size_) { }
Tile::~Tile() { }

Rect<double> Tile::getRect(void){
    return rect_;
}
