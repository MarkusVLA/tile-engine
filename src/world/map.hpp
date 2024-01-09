/**
 * @file map.hpp
 * @author your name (you@domain.com)
 * @brief Game map class to hold all tile objects
 * @version 0.1
 * @date 2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "tile.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class Map {

private:
    std::vector<Tile> tiles_;

public:

    Map() { }
    ~Map() { }
    
    void addTile(Tile tile){
        tiles_.push_back(tile);
    }

    void draw(sf::RenderTarget &target) {
        // Draw all tiles to game window.
        for (auto tile : tiles_) {
            tile.draw(target);
        }
    }

    std::vector<Tile> getTiles(void){
        return tiles_;
    }
};