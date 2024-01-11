/**
 * @file gameobject.hpp
 * @author Markus Lång
 * @brief 
 * @version 0.1
 * @date 2023-11-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */


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
    Vector2<double> pos_;
    Vector2<double> size_;
    Rect<double> rect_;
    std::string textureName_;

public:

    GameObject(Vector2<double> pos, std::shared_ptr<SpriteManager> manager, std::string textureName);
    ~GameObject();

    double getX() const;
    double getY() const;

    Vector2<double> GetPos() { return Vector2<double>(this->getX(), this->getY()); }

    Rect<double> getRect(void) const;

    bool setX(double x);
    bool setY(double y);

    void draw(sf::RenderTarget &target); 
    
    std::ostream friend &operator<<(std::ostream &os, GameObject &object); // Print Game object info

};

GameObject::GameObject(Vector2<double> pos, std::shared_ptr<SpriteManager> manager, std::string textureName): pos_(pos),sprite_manager_(manager), textureName_(textureName) {
    sf::Vector2u s = sprite_manager_->getTexture(textureName_).getSize();
    size_ = Vector2<double>(s.x, s.y);
    rect_ = Rect<double>(pos_, pos_ + size_);
}

GameObject::~GameObject() { }

double GameObject::getX() const { return pos_.GetX(); }
double GameObject::getY() const { return pos_.GetY(); }

Rect<double> GameObject::getRect(void) const { return rect_; }

bool GameObject::setX(double x) { 
    pos_.SetX(x);
    // Update rect
    rect_.SetCornerA(pos_);
    rect_.SetCornerB(pos_ + size_);
    return true;
}
bool GameObject::setY(double y) {
    pos_.SetY(y);
    // Update rect
    rect_.SetCornerA(pos_);
    rect_.SetCornerB(pos_ + size_);
    return true;

}

void GameObject::draw(sf::RenderTarget &target) {
    sprite_manager_->drawSprite(target, textureName_, pos_);
}


std::ostream& operator<<(std::ostream &os, GameObject &object) {
    os << "GameObject: " << "(" << object.pos_.GetX() << ", " << object.pos_.GetY() << ")" << std::endl;
    return os;
}


