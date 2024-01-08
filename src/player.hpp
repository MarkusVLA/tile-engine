
#pragma once
#include "gameobject.hpp"
#include "tile.hpp"

#define PLAYERSIZE sf::Vector2f(16, 16)

class Player: public GameObject {

private:
    sf::Rect<float> playerRect_;

public:
    Player();
    Player(double x, double y, sf::Texture texture);
    ~Player();

    bool checkCollision(double x, double y, const std::vector<Tile> &tiles);
};

Player::Player(): GameObject(), playerRect_(sf::Vector2f(x_, y_), PLAYERSIZE) { } // Default
Player::Player(double x, double y, sf::Texture texture): GameObject(x, y, texture) { }
Player::~Player() { }


bool Player::checkCollision(double x, double y, const std::vector<Tile> &tiles) {
    // Define player's rectangle with updated position
    sf::Rect<float> playerRect(sf::Vector2f(x, y), PLAYERSIZE);

    // Iterate through each tile and check for collision
    for (Tile tile : tiles) {
        sf::Rect<float> tileRect = tile.getRect();

        // Manually check for intersection
        if (playerRect.left + playerRect.width > tileRect.left &&
            playerRect.left < tileRect.left + tileRect.width &&
            playerRect.top + playerRect.height > tileRect.top &&
            playerRect.top < tileRect.top + tileRect.height) {
            // Collision detected
            return true;
        }
    }

    // No collision detected
    return false;
}



