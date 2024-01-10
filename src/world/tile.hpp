
#pragma once
#include "gameobject.hpp"
#include "../utils/vec.h"
#include "../utils/rect.h"
#include "obstacle.h"


class Tile: public GameObject {

private:
    bool visible_;

public:
    Tile(Vector2<double> pos, sf::Texture &texture);
    ~Tile();

    std::vector<Obstacle> getObstacles(void);

    void setVisible(bool visible) { visible_ = visible; }
    bool isVisible(void) { return visible_; }
};

Tile::Tile(Vector2<double> pos, sf::Texture &texture): GameObject(pos , texture) { }
Tile::~Tile() { }


std::vector<Obstacle> Tile::getObstacles(void){
    std::vector<Obstacle> obstacles;
    Vector2<double> cornerA = rect_.GetCornerA();
    Vector2<double> cornerB = rect_.GetCornerB();
    Vector2<double> cornerC(cornerB.GetX(), cornerA.GetY());
    Vector2<double> cornerD(cornerA.GetX(), cornerB.GetY());

    double extension = 16.0;

    // Extend cornerB and cornerD further out by 'extension' units
    Vector2<double> extendedCornerB(cornerB.GetX(), cornerB.GetY() - extension);
    Vector2<double> extendedCornerD(cornerD.GetX(), cornerD.GetY() - extension);

    obstacles.push_back(Obstacle(cornerA, cornerC));
    obstacles.push_back(Obstacle(cornerC, cornerB));
    obstacles.push_back(Obstacle(cornerB, cornerD)); // Use extended points
    obstacles.push_back(Obstacle(cornerD, cornerA));

    return obstacles;
}
