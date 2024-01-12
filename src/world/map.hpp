#pragma once
#include "tile.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <memory> // Include for smart pointers
#include "../camera.hpp"

#ifndef TILESIZE
#define TILESIZE 16
#endif

class Map {
private:
    std::vector<std::unique_ptr<Tile>> tiles_;
    std::shared_ptr<SpriteManager> sprite_manager_;

public:
    Map(std::shared_ptr<SpriteManager> manager) : sprite_manager_(manager) { 
        generateMap();
    }

    ~Map() { }

    void addTile(TileType type, Vector2<double> position) {
        tiles_.push_back(TileFactory::createTile(type, position, sprite_manager_));
    }

    std::vector<Tile*> getTiles() const {
        std::vector<Tile*> rawTiles;
        for (const auto& tilePtr : tiles_) {
            rawTiles.push_back(tilePtr.get());
        }
        return rawTiles;
    }

    void setTileVisibility(Camera cam){
        for (auto& tile : tiles_) {
            tile->setTileVisibility(cam);
        }
    }

    void draw(sf::RenderTarget &target) {
        for (auto& tile : tiles_) {
            tile->draw(target);
        }
    }

    void generateMap() {
        std::cout << "Reading map" << std::endl;
        std::ifstream file("map.csv");
        std::string line;
        int y = 0;

        while (std::getline(file, line)) {
            std::istringstream s(line);
            std::string field;
            int x = 0;
            while (getline(s, field, ',')) {
                TileType type = static_cast<TileType>(std::stoi(field));
                if (type != TileType::AIR) {
                    std::cout << "Adding tile " << static_cast<int>(type) << " to " << x * TILESIZE << ", " << y * TILESIZE << std::endl;
                    addTile(type, Vector2<double>(x * TILESIZE, y * TILESIZE));
                }
                x++;
            }
            y++;
        }
    }
};
