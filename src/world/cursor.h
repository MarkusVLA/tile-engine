

#pragma once
#include "../utils/vec.h"
#include "../utils/sprite_manager.h"

class Cursor {
private:
    Vector2<double> pos_;
    std::shared_ptr<SpriteManager> manager_;


public:
    Vector2<double> getPos(void){ return pos_; }
    void findPos(sf::Window window){
        
    }

    void draw(sf::RenderTarget target){
        manager_->drawSprite(target, "cursor", pos_);
    }

};