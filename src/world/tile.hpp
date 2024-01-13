#pragma once

#include "gameobject.hpp"
#include "../utils/vec.h"
#include "../utils/rect.h"
#include "obstacle.h"
#include "../camera.hpp"
#include <map>


enum class TileType {

    AIR =           0,
    BACK_WALL =     1,
    LEFT_WALL =     2,
    RIGHT_WALL =    3,
    FRONT_WALL =    4,
    CENTER_WALL =   5,
    LEFT_CORNER =   6,
    RIGHT_CORNER =  7,
    CHEST =         8,
    LAVA =          9

};

class Tile: public GameObject {
    
protected:
    TileType type_;
    bool isVisible_;
public:
    Tile(Vector2<double> pos, TileType type, std::shared_ptr<SpriteManager> manager)
        : GameObject(pos, manager, tileTypeToString(type)), type_(type) { isVisible_ = false; }

    virtual ~Tile() {}

    virtual std::vector<Obstacle> getObstacles(void) = 0;

    std::string tileTypeToString(TileType type) {

        switch (type) {
            case TileType::AIR: return              "floor";
            case TileType::BACK_WALL: return        "back";
            case TileType::LEFT_WALL: return        "left";
            case TileType::RIGHT_WALL: return       "right";
            case TileType::FRONT_WALL: return       "front";
            case TileType::LEFT_CORNER: return      "left_corner";
            case TileType::RIGHT_CORNER: return     "right_corner";
            case TileType::CENTER_WALL: return      "center";
            case TileType::CHEST: return            "chest";
            default: return                         "default";
        }
    }


    bool isVisible() {return isVisible_; }

    void setTileVisibility(Camera cam){
        // Set the tiles visibility.
        Rect<double> camviewRect = cam.getViewRect();
        camviewRect.stretch(Vector2<double>(100, 100)); // Make the cam view rect a bit bigger so light don't glitch out. 
        if (camviewRect.Intersects(rect_)){
            isVisible_ = true;
        } else {
            isVisible_ = false;
        }
    }
    
    virtual void draw(sf::RenderTarget &target){
        if (isVisible_){
            sprite_manager_->drawSprite(target, textureName_, pos_);
        }
    }
};

class BasicTile: public Tile {
public:
    BasicTile(Vector2<double> pos, TileType type, std::shared_ptr<SpriteManager> manager) : Tile(pos, type, manager) { }

    std::vector<Obstacle> getObstacles(void) override {
        std::vector<Obstacle> obstacles;
        Vector2<double> cornerA = rect_.GetCornerA();
        Vector2<double> cornerB = rect_.GetCornerB();
        Vector2<double> cornerC(cornerB.GetX(), cornerA.GetY());
        Vector2<double> cornerD(cornerA.GetX(), cornerB.GetY());
        // double extension = 24.0;
        // Vector2<double> extendedCornerB(cornerB.GetX(), cornerB.GetY() - extension);
        // Vector2<double> extendedCornerD(cornerD.GetX(), cornerD.GetY() - extension);
        obstacles.push_back(Obstacle(cornerA, cornerC));
        obstacles.push_back(Obstacle(cornerC, cornerB));
        obstacles.push_back(Obstacle(cornerB, cornerD));
        obstacles.push_back(Obstacle(cornerD, cornerA));
        return obstacles;
    }
};

class FrontWall: public Tile {

public:

    FrontWall(Vector2<double> pos, std::shared_ptr<SpriteManager> manager) 
        : Tile(pos, TileType::FRONT_WALL, manager) {
    }

    std::vector<Obstacle> getObstacles(void) override {
        return std::vector<Obstacle>(); // No obsticles
    }

};


class LavaTile: public Tile {

public:
    LavaTile(Vector2<double> pos, std::shared_ptr<SpriteManager> manager) 
        : Tile(pos, TileType::LAVA, manager) {
    }

    std::vector<Obstacle> getObstacles(void) override {
        std::vector<Obstacle> obstacles;
        return obstacles;
    }

};

class Container: public Tile {

public:

    Container(Vector2<double> pos, std::shared_ptr<SpriteManager> manager)
        : Tile(pos, TileType::CHEST, manager) {       
    }

    std::vector<Obstacle> getObstacles(void) override {
        return std::vector<Obstacle>(); // No obsticles
    }

    void interact(){
        std::cout << "I am a chest =)" << std::endl;
    }
    
};


class TileFactory {

public:
    static std::unique_ptr<Tile> createTile(TileType type, Vector2<double> pos, std::shared_ptr<SpriteManager> manager) {
        switch (type) {
            case TileType::LAVA:            return std::make_unique<LavaTile>(pos, manager);
            case TileType::CHEST:           return std::make_unique<Container>(pos, manager);
            case TileType::FRONT_WALL:      return std::make_unique<FrontWall>(pos, manager);
            default:                        return std::make_unique<BasicTile>(pos, type, manager);
        }
    }
};