
#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <sstream>
#include "vec.h"
#include "../assets/textures/atlas.h" // header file with the atlas data

#define SPRITE_SIZE 16

class SpriteManager {
private:
    sf::Texture textureAtlas;
    std::map<std::string, sf::IntRect> spritePositions;

    void loadTextureAtlas() {
        // Load the texture atlas from the embedded data
        if (!textureAtlas.loadFromMemory(textureAtlasData, sizeof(textureAtlasData))) {
            std::cerr << "Failed to load texture atlas from memory" << std::endl;
            return;
        }

        // Parse the sprite position data from the embedded CSV data
        for (int i = 0; texturePositionsData[i] != nullptr; ++i) {
            std::string line(texturePositionsData[i]);
            std::stringstream linestream(line);
            std::string name;
            int x, y;
            char comma;

            std::getline(linestream, name, ',');
            linestream >> x >> comma >> y;

            spritePositions[name] = sf::IntRect({x, y}, {SPRITE_SIZE, SPRITE_SIZE});
        }
    }

public:
    SpriteManager() {   
        std::cout << "Reading texture atlas" << std::endl;
        loadTextureAtlas(); // Call the new loadTextureAtlas function
    }

    void drawSprite(sf::RenderTarget &target, const std::string& spriteName, const Vector2<double> drawPos) {
        std::string name = spriteName;
        if (spritePositions.find(spriteName) == spritePositions.end()) {
            name = "default";
        }
        sf::Sprite sprite(textureAtlas, spritePositions[name]);
        sprite.setPosition(drawPos.toSF_Vectorf());
        target.draw(sprite);
    }
};
