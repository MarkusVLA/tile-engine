
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <ostream>
#include "../utils/vec.h"
#include "../utils/rect.h"
#include "../utils/sprite_manager.h"
class GameObject {
protected:
    std::shared_ptr<SpriteManager> sprite_manager_;
    std::string textureName_;
    Vector2<double> pos_;
    Vector2<double> size_;
    Rect<double> rect_;
public:
    GameObject(Vector2<double> pos, std::shared_ptr<SpriteManager> manager, std::string textureName, Vector2<double> rectSize = Vector2<double>(16, 16) )
        : pos_(pos),sprite_manager_(manager), textureName_(textureName) {
        size_ = Vector2<double>(rectSize);
        rect_ = Rect<double>(pos_, pos_ + size_);
    }

    ~GameObject() { }
    double getX() const { return pos_.GetX(); }
    double getY() const { return pos_.GetY(); }
    Vector2<double> getPosition() { return pos_; }
    Rect<double> getRect(void) const { return rect_; }

    void setX(double x) { 
        pos_.SetX(x);
        rect_.SetCornerA(pos_);
        rect_.SetCornerB(pos_ + size_);
    }

    void setY(double y) {
        pos_.SetY(y);
        rect_.SetCornerA(pos_);
        rect_.SetCornerB(pos_ + size_);
    }

    void setPosition(Vector2<double> pos){
        setX(pos.GetX());
        setY(pos.GetY());
    }

    friend std::ostream& operator<<(std::ostream &os, GameObject &object) {
        os << "GameObject: " << "(" << object.pos_.GetX() << ", " << object.pos_.GetY() << ")" << std::endl;
        return os;
    }

};

