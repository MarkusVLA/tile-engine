
#pragma once
#include "gameobject.hpp"

#define TILESIZE 16

class Tile: public GameObject {

private:
    sf::Rect<float> rect_;

public:
    Tile();
    Tile(double x, double y, sf::Texture texture);
    ~Tile();

    sf::Rect<float> getRect(void);
};

Tile::Tile(): GameObject() { } // Default
Tile::Tile(double x, double y, sf::Texture texture): GameObject(x, y, texture), rect_(sf::Vector2f(x_, y_), sf::Vector2f(TILESIZE, TILESIZE)) { }
Tile::~Tile() { }

sf::Rect<float> Tile::getRect(void){
    return rect_;
}
