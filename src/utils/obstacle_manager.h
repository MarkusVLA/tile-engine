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

    const std::vector<Obstacle>& getObstacles() const {
        return obstacles;
    }

    void draw(sf::RenderWindow& window) {
        for (auto obstacle : obstacles) {
            obstacle.draw(window);
        }
    }
};
