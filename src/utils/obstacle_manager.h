#pragma once

#include "../world/obstacle.h"
#include "../world/tile.hpp"
#include <vector>

class ObstacleManager {
private:
    std::vector<Obstacle> obstacles;

public:
    void addObstacle(const Obstacle& obstacle) {
        obstacles.push_back(obstacle);
    }

    void buildObstacleMap(Map& map) {
        obstacles.clear();
        const auto& tiles = map.getTiles();
        for (auto tile : tiles) {
            Vector2<double> start = tile->getRect().GetCornerA();
            Vector2<double> end = tile->getRect().GetCornerB();
            
            if (tile->isVisible()) {
                std::vector<Obstacle> obstacles = tile->getObstacles();
                for (auto obstacle : obstacles) {
                    addObstacle(obstacle);
                }
            }
        }
    }

    const std::vector<Obstacle>& getObstacles() const {
        return obstacles;
    }

    void draw(sf::RenderTarget &target) {
        for (auto obstacle : obstacles) {
            obstacle.draw(target);
        }
    }
};
