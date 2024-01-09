#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "gameobject.hpp"
#include "tile.hpp"
#include "utils/vec.h"
#include "utils/rect.h"


class Floor : public GameObject {
private:
    std::vector<Tile> tiles; // Container for tiles
    int tileSize;

public:
    // Constructor
    Floor(sf::Texture &texture, int width = 100000, int height = 100000)
        : GameObject(Vector2<double>(0, 0), texture), tileSize(512 + 256) {
        // Half of the width and height to generate tiles in all directions
        int halfWidth = width / 2;
        int halfHeight = height / 2;

        // Generate a fixed set of tiles covering both positive and negative coordinates
        for (int y = -halfHeight; y < halfHeight; y += tileSize) {
            for (int x = -halfWidth; x < halfWidth; x += tileSize) {
                Vector2<double> tilePos(x, y);
                tiles.emplace_back(tilePos, texture_);
            }
        }
    }


    void Floor::updateVisibleTiles(const sf::View& cameraView) {
        // Calculate the bounds of the visible world area
        sf::Vector2f center = cameraView.getCenter();
        sf::Vector2f size = cameraView.getSize();
        Rect<double> visibleArea(Vector2<double>(center.x - size.x / 2, center.y - size.y / 2),
                                Vector2<double>(center.x + size.x / 2, center.y + size.y / 2));

        for (auto& tile : tiles) {
            // Use the getRect method of the tile to get its Rect
            Rect<double> tileRect = tile.getRect();

            // Check if the tile is within the visible area
            if (visibleArea.Intersects(tileRect)) {
                tile.setVisible(true);
            } else {
                tile.setVisible(false);
            }
        }
    }



    // Draw method to draw all visible tiles
    void draw(sf::RenderTarget &target) {
        for (auto& tile : tiles) {
            if (tile.isVisible()) {
                tile.draw(target);
            }
        }
    }
};

