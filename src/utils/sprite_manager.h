#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <fstream>
#include <sstream>
#include "vec.h"

#define SPRITE_SIZE 16 // Size of the sprite without padding

class SpriteManager {
private:
    sf::Texture textureAtlas;
    std::map<std::string, sf::IntRect> spritePositions;
    sf::IntRect textureSampler;

    void loadTextureAtlas(const std::string& atlasPath, const std::string& csvPath) {
        if (!textureAtlas.loadFromFile(atlasPath)) {
            std::cerr << "Failed to load texture atlas" << std::endl;
            return;
        }


        std::ifstream file(csvPath);
        std::string line, name;
        int x, y;
        while (std::getline(file, line)) {
            std::stringstream linestream(line);
            std::getline(linestream, name, ',');
            
            if (!(linestream >> x)) {
                std::cerr << "Error reading X coordinate for sprite " << name << std::endl;
                continue;
            }
            char comma;
            linestream >> comma;
            if (!(linestream >> y)) {
                std::cerr << "Error reading Y coordinate for sprite " << name << std::endl;
                continue;
            }

            std::cout << "Sprite: " << name << ", X: " << x << ", Y: " << y << std::endl;
            spritePositions[name] = sf::IntRect({x, y}, {SPRITE_SIZE, SPRITE_SIZE});
        }
    }

public:
    SpriteManager() {   
        std::cout << "Reading texture atlas" << std::endl;
        loadTextureAtlas("../src/assets/textures/texture_atlas.png", "../src/assets/textures/texture_positions.csv");
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