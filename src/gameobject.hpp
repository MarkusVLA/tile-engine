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

class GameObject {

protected:

    sf::Texture texture_;
    sf::Sprite sprite_;
    std::string id_;
    double x_, y_;

public:

    GameObject();
    GameObject(double x, double y, sf::Texture texture);
    ~GameObject();

    double getX() const;
    double getY() const;

    bool setX(double x);
    bool setY(double y);

    void updateSpritePos(void);

    void draw(sf::RenderWindow &window); // Draw game object on a surface

    std::ostream friend &operator<<(std::ostream &os, GameObject &object); // Print Game object info

};
GameObject::GameObject(): x_(0), y_(0), sprite_(sf::Sprite(texture_)) {
    texture_.loadFromFile("assets/default.png");
}

GameObject::GameObject(double x, double y, sf::Texture texture): x_(x), y_(y), texture_(texture), sprite_(sf::Sprite(texture)), id_("object id") {

}

GameObject::~GameObject() { }

double GameObject::getX() const { return x_; }
double GameObject::getY() const { return y_; }

bool GameObject::setX(double x) { x_ = x; return true;}
bool GameObject::setY(double y) { y_ = y; return true;}

void GameObject::updateSpritePos(){ sprite_.setPosition(sf::Vector2f(x_, y_)); }

void GameObject::draw(sf::RenderWindow &window) {
    window.draw(sprite_); 
}

std::ostream& operator<<(std::ostream &os, GameObject &object) {
    os << "GameObject: " << object.id_ << "(" << object.x_ << ", " << object.y_ << ")" << std::endl;
    return os;
}


