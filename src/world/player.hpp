
#pragma once
#include "gameobject.hpp"
#include "tile.hpp"
#include "utils/vec.h"

#define PLAYERSIZE sf::Vector2f(16, 16)

class Player: public GameObject {

private:
    sf::Rect<float> playerRect_;

public:
    Player();
    Player(Vector2<double> pos, sf::Texture texture);
    ~Player();
};

Player::Player(): GameObject(), playerRect_(sf::Vector2f(), PLAYERSIZE) { } // Default
Player::Player(Vector2<double> pos, sf::Texture texture): GameObject(pos, texture) { }
Player::~Player() { }




