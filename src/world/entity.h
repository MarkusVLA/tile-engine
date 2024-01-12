
#pragma once


#include <SFML/Graphics.hpp>
#include "gameobject.hpp"
#include "../utils/vec.h"
#include "map.hpp"


class Entity: public GameObject {

private:
    Vector2<double> facing_;
    double speed_;
    double animationFrame_;
    int numFrames_;


public:
    Entity(Vector2<double> pos, std::shared_ptr<SpriteManager> sManager, std::string spriteID)
        : GameObject(pos, sManager, spriteID) {
            speed_ = 1;
            animationFrame_ = 0;
            facing_ = Vector2<double>();
        }
    
    void UpdateFrame(double dt) { animationFrame_ += dt; }


    void move(Vector2<double> dir, Map& gameMap){
        // Set facing
        facing_ = dir.Normalize();
        animationFrame_ += 15.0f / 120.0f;
        Vector2<double> delta = dir.Normalize() * speed_;
        Rect<double> newRect = getRect();
        newRect.move(delta);
        for (auto& tile: gameMap.getTiles()) {
            Rect<double> tileRect = tile.getRect();
            // Chek if new pos intersects with the tile and scale newPos accordingly
            if (newRect.Intersects(tileRect)){
                return;
            }
        }

        setPosition(pos_ + delta);
    }

    void draw(sf::RenderTarget &target) {
        int frameIndex = static_cast<int>(floor(animationFrame_)) % numFrames_;
        sprite_manager_->drawSprite(target, textureName_, pos_);
    }
    
};