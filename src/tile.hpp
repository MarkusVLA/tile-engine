
#pragma once
#include "gameobject.hpp"


class Tile: public GameObject {

private:
    /* data */
public:
    Tile();
    Tile(double x, double y, sf::Texture texture);
    ~Tile();
};

Tile::Tile(): GameObject() { } // Default
Tile::Tile(double x, double y, sf::Texture texture): GameObject(x, y, texture) { }
Tile::~Tile() { }
