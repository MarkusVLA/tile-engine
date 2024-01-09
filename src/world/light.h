#pragma once

#include <SFML/Graphics.hpp>
#include "../utils/vec.h"
#include "../utils/obstacle_manager.h" 
#include "../utils/ray.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Light {
private:
    Vector2<double> position;
    int numRays;
    double radius;
    std::vector<Vector2<double>> endPoints;  
    float angleIncrement;  


public:
    Light(Vector2<double> position): position(position), numRays(360) {
        angleIncrement = 360.0f / static_cast<float>(numRays);
    }
    Light(Vector2<double> position, int numRays): position(position), numRays(numRays) {
        angleIncrement = 360.0f / static_cast<float>(numRays);
    }

    double getRadius() const {
        return radius;
    }


    Vector2<double> getPosition() const {
        return position;
    }

    void setPosition(Vector2<double> position) {
        this->position = position;
    }

    void castRays(const ObstacleManager& manager) {
        const auto& obstacles = manager.getObstacles();
        endPoints.clear();

        float angleIncrement = 360.0f / static_cast<float>(numRays);

        for (int i = 0; i < numRays; ++i) {
            float angle = static_cast<float>(i) * angleIncrement * static_cast<float>(M_PI) / 180.0f;
            Ray ray(position, angle);
            Vector2<double> endPoint = ray.cast(obstacles);
            endPoints.push_back(endPoint);
        }
    }

    const std::vector<Vector2<double>>& getEndPoints() const {
        return endPoints;
    }
    


void drawDebug(sf::RenderTarget& target, sf::Shader& shader) {
    for (const auto &endPoint : endPoints) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(static_cast<float>(position.GetX()), static_cast<float>(position.GetY())), sf::Color::Yellow),
            sf::Vertex(sf::Vector2f(static_cast<float>(endPoint.GetX()), static_cast<float>(endPoint.GetY())), sf::Color::Yellow)
        };
        
        target.draw(line, 2, sf::PrimitiveType::Lines, &shader);
    }
}



    friend std::ostream& operator<<(std::ostream& os, const Light& light) {
        os << "Light: " << light.position << std::endl;
        return os;
    }

};


