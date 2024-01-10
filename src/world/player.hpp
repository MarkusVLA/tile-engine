
#pragma once
#include "gameobject.hpp"
#include "tile.hpp"
#include "../utils/vec.h"
#include "../utils/rect.h"
#include "map.hpp"
#include "bullet.h"

#define PLAYERSIZE sf::Vector2f(16, 16)

class Player: public GameObject {

private:
    Vector2<double> facing_;
    bool running_;
    int frame_;

public:
    Player(Vector2<double> pos, sf::Texture &texture): GameObject(pos, texture) { }
    ~Player() { }


    std::shared_ptr<Bullet> shootBullet(Vector2<double> dir, std::shared_ptr<LightMap> lMap) {
        return std::make_shared<Bullet>(pos_, texture_, dir, lMap);
    }

    bool checkCollisionWithMap(Map& gameMap) {
        Rect<double> playerRect = getRect(); 
        for (auto tile : gameMap.getTiles()) {
            Rect<double> tileRect = tile.getRect();  
            if (playerRect.Intersects(tileRect)) return true; 
        return false;  
        }
    }

    bool checkCollisionWithMap(Map& gameMap, double potentialX, double potentialY) {
        Rect<double> playerRect = getRect(); 
        Vector2<double> delta(potentialX - playerRect.GetCornerA().GetX(), potentialY - playerRect.GetCornerA().GetY());
        playerRect.move(delta); // Calculate the new position of the player rectangle
        for (auto tile : gameMap.getTiles()) {
            Rect<double> tileRect = tile.getRect();  
            if (playerRect.Intersects(tileRect)) {
                return true;  
            }
        }
        return false;  
    }
};