#pragma once

#include "gameobject.hpp"
#include "../utils/vec.h"
#include "../utils/rect.h"
#include "obstacle.h"

enum class TileType {
    AIR = 0,
    BACK_WALL = 1,
    LEFT_WALL = 2,
    RIGHT_WALL = 3,
    FRONT_WALL = 4,
    CENTER_WALL = 5,
    LEFT_CORNER = 6,
    RIGHT_CORNER = 7
};

class Tile: public GameObject {

private:
    TileType type_;

public:

    Tile(Vector2<double> pos, TileType type, std::shared_ptr<SpriteManager> manager)
        : GameObject(pos, manager, tileTypeToString(type)), type_(type) { }


    ~Tile() { }


    std::vector<Obstacle> getObstacles(void){
        std::vector<Obstacle> obstacles;

        if (type_ == TileType::FRONT_WALL){
            return obstacles;
        }

        Vector2<double> cornerA = rect_.GetCornerA();
        Vector2<double> cornerB = rect_.GetCornerB();
        Vector2<double> cornerC(cornerB.GetX(), cornerA.GetY());
        Vector2<double> cornerD(cornerA.GetX(), cornerB.GetY());

        double extension = 24.0;

        // Extend cornerB and cornerD further out by 'extension' units
        Vector2<double> extendedCornerB(cornerB.GetX(), cornerB.GetY() - extension);
        Vector2<double> extendedCornerD(cornerD.GetX(), cornerD.GetY() - extension);

        obstacles.push_back(Obstacle(cornerA, cornerC));
        obstacles.push_back(Obstacle(cornerC, cornerB));
        obstacles.push_back(Obstacle(cornerB, cornerD)); // Use extended points
        obstacles.push_back(Obstacle(cornerD, cornerA));


        return obstacles;
    }


    std::string tileTypeToString(TileType type) {

        switch (type) {

            case TileType::BACK_WALL: return "back";
            case TileType::LEFT_WALL: return "left";
            case TileType::RIGHT_WALL: return "right";
            case TileType::FRONT_WALL: return "front";
            case TileType::LEFT_CORNER: return "left_corner";
            case TileType::RIGHT_CORNER: return "right_corner";
            case TileType::CENTER_WALL: return "center";

            default: return "default";
        }
    }

};