
#pragma once
#include "gameobject.hpp"


class Player: public GameObject {

private:
    /* data */
public:
    Player();
    Player(double x, double y, sf::Texture texture);
    ~Player();
};

Player::Player(): GameObject() { } // Default
Player::Player(double x, double y, sf::Texture texture): GameObject(x, y, texture) { }
Player::~Player() { }
