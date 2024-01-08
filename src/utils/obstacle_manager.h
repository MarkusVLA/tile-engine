#pragma once

#include "../world/obstacle.h"
#include <vector>

class ObstacleManager {
private:
    std::vector<Obstacle> obstacles;

public:
    void addObstacle(const Obstacle& obstacle) {
        obstacles.push_back(obstacle);
    }

    void buildObstacleMap(Map& map) {
        const auto& tiles = map.getTiles();

        for (auto tile : tiles) {
            Vector2<double> start = tile.getRect().GetCornerA();
            Vector2<double> end = tile.getRect().GetCornerB();
            
            std::vector<Obstacle> obstacles = tile.getObstacles();
            for (auto obstacle : obstacles) {
                addObstacle(obstacle);
            }
        }
    }

    const std::vector<Obstacle>& getObstacles() const {
        return obstacles;
    }

    void draw(sf::RenderWindow& window) {
        for (auto obstacle : obstacles) {
            obstacle.draw(window);
        }
    }
};
