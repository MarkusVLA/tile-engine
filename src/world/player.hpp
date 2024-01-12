
#pragma once
#include "gameobject.hpp"
#include "entity.h"
#include "tile.hpp"
#include "../utils/vec.h"
#include "../utils/rect.h"
#include "map.hpp"
#include "bullet.h"

#define NUM_FRAMES 3

class Player: public Entity {


public:
    Player(Vector2<double> pos, std::shared_ptr<SpriteManager> manager)
    : Entity(pos, manager, "player1") { }
    ~Player() { }

    std::shared_ptr<Bullet> shootBullet(Vector2<double> dir, std::shared_ptr<LightMap> lMap) {
        return std::make_shared<Bullet>(pos_, sprite_manager_, dir, lMap);
    }
};