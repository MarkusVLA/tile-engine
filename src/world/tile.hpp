
#pragma once
#include "gameobject.hpp"
#include "utils/vec.h"
#include "utils/rect.h"
#include "obstacle.h"

#define TILESIZE 16

class Tile: public GameObject {

public:
    Tile();
    Tile(Vector2<double> pos, sf::Texture texture);
    ~Tile();

    std::vector<Obstacle> getObstacles(void);
};

Tile::Tile(): GameObject() { } // Default
Tile::Tile(Vector2<double> pos, sf::Texture texture): GameObject(pos , texture) { }
Tile::~Tile() { }


std::vector<Obstacle> Tile::getObstacles(void){
    std::vector<Obstacle> obstacles;
    
    Vector2<double> cornerA = rect_.GetCornerA();
    Vector2<double> cornerB = rect_.GetCornerB();
    Vector2<double> cornerC(cornerB.GetX(), cornerA.GetY());
    Vector2<double> cornerD(cornerA.GetX(), cornerB.GetY());
  
    obstacles.push_back(Obstacle(cornerA, cornerC));
    obstacles.push_back(Obstacle(cornerC, cornerB));
    obstacles.push_back(Obstacle(cornerB, cornerD));
    obstacles.push_back(Obstacle(cornerD, cornerA));

    return obstacles;
}