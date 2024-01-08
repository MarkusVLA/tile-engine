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
#include "utils/vec.h"

class GameObject {

protected:

    sf::Texture texture_;
    sf::Sprite sprite_;
    std::string id_;
    Vector2<double> pos_;

public:

    GameObject();
    GameObject(Vector2<double> pos, sf::Texture texture);
    ~GameObject();

    double getX() const;
    double getY() const;

    bool setX(double x);
    bool setY(double y);

    void updateSpritePos(void);

    void draw(sf::RenderWindow &window); // Draw game object on a surface

    std::ostream friend &operator<<(std::ostream &os, GameObject &object); // Print Game object info

};
GameObject::GameObject(): pos_(Vector2<double>()), sprite_(sf::Sprite(texture_)) {
    texture_.loadFromFile("assets/default.png");
}

GameObject::GameObject(Vector2<double> pos, sf::Texture texture): pos_(pos), texture_(texture), sprite_(sf::Sprite(texture)), id_("object id") {

}

GameObject::~GameObject() { }

double GameObject::getX() const { return pos_.GetX(); }
double GameObject::getY() const { return pos_.GetY(); }

bool GameObject::setX(double x) { pos_.SetX(x); return true;}
bool GameObject::setY(double y) { pos_.SetY(y); return true;}

void GameObject::updateSpritePos(){ sprite_.setPosition(sf::Vector2f(static_cast<float>(pos_.GetX()), static_cast<float>(pos_.GetY()))); }

void GameObject::draw(sf::RenderWindow &window) {
    window.draw(sprite_); 
}

std::ostream& operator<<(std::ostream &os, GameObject &object) {
    os << "GameObject: " << object.id_ << "(" << object.pos_.GetX() << ", " << object.pos_.GetY() << ")" << std::endl;
    return os;
}


