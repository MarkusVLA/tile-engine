#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include "vec.h"
#include <vector>


class SpriteManager {

private:

    std::map<std::string, sf::Texture> textures;

    void loadTextures() {

    if (!textures["tile"].loadFromFile("../src/assets/textures/tile.png")) {
        std::cerr << "Failed to load tile texture" << std::endl;
    }

    if (!textures["default"].loadFromFile("../src/assets/textures/default.png")) {
        std::cerr << "Failed to load default texture" << std::endl;
    }

    if (!textures["floor"].loadFromFile("../src/assets/textures/floor.png")) {
        std::cerr << "Failed to load floor texture" << std::endl;
    }

    if (!textures["cursor"].loadFromFile("../src/assets/textures/cursor.png")) {
        std::cerr << "Failed to load cursor texture" << std::endl;
    }
}

public:

    SpriteManager(){
        std::cout << "Loading textures" << std::endl;
        loadTextures();
        std::cout << "Created sprite manager" << std::endl;
    }

    sf::Texture getTexture(std::string texName){ return textures[texName]; }

    void drawSprite(sf::RenderTarget &target, std::string texName, Vector2<double> drawPos){
        // Create sprite from texture and draw it in the correct position
        sf::Sprite sprite(textures[texName]);
        sprite.setPosition(drawPos.toSF_Vectorf());
        target.draw(sprite);
    }

    void ping(void){
        std::cout << "Spritemanager: pong" << std::endl;
    }

};