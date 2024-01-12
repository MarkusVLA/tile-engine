#pragma once

#include "entity.h"
#include "map.hpp"
#include "../utils/vec.h"


class Enemy: public Entity {

private:
    Vector2<double> target_; // Target enmy moves towards.

public:

    Enemy(Vector2<double> pos, std::shared_ptr<SpriteManager> manager)
        : Entity(pos, manager, "enemy") 
    {
        speed_ = 0.3;
    }

    Vector2<double> getTarget(void){ return target_; }
    void setTarget(Vector2<double> target){ target_ = target; }

    void follow(Map &gameMap){
        // Find direction to target and move there.
        // TODO Better enemy follow logic. maybe A* or similar
        Vector2<double> dir = target_ - pos_;
        move(dir, gameMap);
    }

};