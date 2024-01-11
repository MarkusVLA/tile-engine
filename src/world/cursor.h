#pragma once


#include "../utils/vec.h"
#include "../utils/sprite_manager.h"

class Cursor {
private:
    Vector2<double> pos_;
    std::string textureName_;
    std::shared_ptr<SpriteManager> sprite_manager_;


public:

    Cursor(std::shared_ptr<SpriteManager> manager): sprite_manager_(manager), textureName_("cursor") { }

    Vector2<double> getPos(void){ return pos_; }

    void setPos(Vector2<double> pos){ pos_ = pos; }

    void draw(sf::RenderTarget &target){
        sprite_manager_->drawSprite(target, textureName_, pos_);
    }

};