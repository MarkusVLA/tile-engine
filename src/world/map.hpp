#pragma once

#include "tile.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include "tile.hpp"


#ifndef TILESIZE
#define TILESIZE 16
#endif


class Map {
private:
    std::vector<Tile> tiles_;
    std::shared_ptr<SpriteManager> sprite_manager_;

public:

    Map(std::shared_ptr<SpriteManager> manager) : sprite_manager_(manager) { 
        generateMap();
    }

    ~Map() { }

    void addTile(Tile tile) {
        tiles_.push_back(tile);
    }

    std::vector<Tile> getTiles() const {
        return tiles_;
    }

    void draw(sf::RenderTarget &target) {
        for (auto& tile : tiles_) {
            tile.draw(target);
        }
    }


   void generateMap() {
        std::cout << "Reading map" << std::endl;
        std::ifstream file("map.csv"); // Replace with your CSV file path
        std::string line;
        int y = 0;

        while (std::getline(file, line)) {
            std::istringstream s(line);
            std::string field;
            int x = 0;
            while (getline(s, field, ',')) {
                TileType type = static_cast<TileType>(std::stoi(field));
                if (type != TileType::AIR){
                    std::cout << "Adding tile " << static_cast<int>(type) << " to " << x*TILESIZE << ", " << y*TILESIZE << std::endl;
                    addTile(Tile(Vector2<double>(x * TILESIZE, y * TILESIZE), type, sprite_manager_));
                }

                x++;
            }
            y++;
        }
    }
};
