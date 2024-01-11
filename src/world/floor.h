#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "gameobject.hpp"
#include "../utils/vec.h"
#include "../utils/rect.h"

class Floor : public GameObject {
private:
    std::vector<Vector2<double>> tilePositions; // Container for tile positions
    std::vector<bool> tileVisibility; // Visibility of each tile
    int tileSize;

public:
    // Constructor
    Floor(std::shared_ptr<SpriteManager> manager, int width = 10000, int height = 10000)
        : GameObject(Vector2<double>(0, 0), manager, "floor"), tileSize(512) {
        int halfWidth = width / 2;
        int halfHeight = height / 2;

        for (int y = -halfHeight; y < halfHeight; y += tileSize) {
            for (int x = -halfWidth; x < halfWidth; x += tileSize) {
                tilePositions.emplace_back(Vector2<double>(x, y));
                tileVisibility.push_back(false); // Initially, all tiles are not visible
            }
        }
    }

    void updateVisibleTiles(const sf::View& cameraView) {
        sf::Vector2f center = cameraView.getCenter();
        sf::Vector2f size = cameraView.getSize();
        Rect<double> visibleArea(Vector2<double>(center.x - size.x / 2, center.y - size.y / 2),
                                 Vector2<double>(center.x + size.x / 2, center.y + size.y / 2));

        for (size_t i = 0; i < tilePositions.size(); ++i) {
            Rect<double> tileRect(tilePositions[i], Vector2<double>(tilePositions[i].GetX() + tileSize, tilePositions[i].GetY() + tileSize));
            tileVisibility[i] = visibleArea.Intersects(tileRect);
        }
    }

    void draw(sf::RenderTarget &target) {
        for (size_t i = 0; i < tilePositions.size(); ++i) {
            if (tileVisibility[i]) {
                sprite_manager_->drawSprite(target, textureName_, tilePositions[i]);
            }
        }
    }
};
