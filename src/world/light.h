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
    std::vector<Vector2<double>> endPoints;  // Stores the end points of all rays
    float angleIncrement;  // Added angle increment as a member

public:
    Light(Vector2<double> position): position(position), numRays(360) {
        angleIncrement = 360.0f / static_cast<float>(numRays);
    }
    Light(Vector2<double> position, int numRays): position(position), numRays(numRays) {
        angleIncrement = 360.0f / static_cast<float>(numRays);
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

     void draw(sf::RenderWindow& window) {
        std::vector<sf::Vertex> raysVertices;
        raysVertices.reserve(numRays * 2);

        for (int i = 0; i < numRays; ++i) {

            Vector2<double> endPoint = endPoints[i];

            sf::Vertex startVertex(sf::Vector2f(position.GetX(), position.GetY()), sf::Color::Yellow);
            sf::Vertex endVertex(sf::Vector2f(static_cast<float>(endPoint.GetX()), static_cast<float>(endPoint.GetY())), sf::Color::Yellow);

            raysVertices.push_back(startVertex);
            raysVertices.push_back(endVertex);
        }

        window.draw(raysVertices.data(), raysVertices.size(), sf::PrimitiveType::Lines);
    }


};
