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
    std::vector<Vector2<double>> endPoints;  
    float angleIncrement;  
    sf::Shader shader;
    sf::RenderTexture lightRenderTexture;
    bool textureInitialized = false;



public:
    Light(Vector2<double> position): position(position), numRays(360) {
        angleIncrement = 360.0f / static_cast<float>(numRays);
        
        if (!shader.loadFromFile("../src/shaders/light.frag", sf::Shader::Fragment)) {
            std::cout << "Error loading shader" << std::endl;
        }
    }
    Light(Vector2<double> position, int numRays): position(position), numRays(numRays) {
        angleIncrement = 360.0f / static_cast<float>(numRays);

        if (!shader.loadFromFile("../src/shaders/light.frag", sf::Shader::Fragment)) {
            std::cout << "Error loading shader" << std::endl;
        }
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

            sf::Vertex startVertex(sf::Vector2f(static_cast<float>(position.GetX()), static_cast<float>(position.GetY())), sf::Color::Yellow);
            sf::Vertex endVertex(sf::Vector2f(static_cast<float>(endPoint.GetX()), static_cast<float>(endPoint.GetY())), sf::Color::Yellow);

            raysVertices.push_back(startVertex);
            raysVertices.push_back(endVertex);
        }

        window.draw(raysVertices.data(), raysVertices.size(), sf::PrimitiveType::Lines);
    }

    void initializeRenderTexture(const sf::RenderWindow& window) {
        if (!lightRenderTexture.create({window.getSize().x, window.getSize().y})) {
            throw std::runtime_error("Failed to create render texture for light.");
        }
        textureInitialized = true;
    }
    

    void drawFail(sf::RenderWindow& window) {
        if (!textureInitialized) {
            initializeRenderTexture(window);
        }
        

        // Create light shape
        sf::ConvexShape lightShape;
        lightShape.setPointCount(endPoints.size());
        for (size_t i = 0; i < endPoints.size(); ++i) {
            lightShape.setPoint(i, sf::Vector2f(static_cast<float>(endPoints[i].GetX()), static_cast<float>(endPoints[i].GetY())));
        }

        // Set shader parameters
        shader.setUniform("LightPos", sf::Vector2f(position.GetX(), window.getSize().y - position.GetY()));
        shader.setUniform("LightIntensity", 0.7f);

        // Draw light shape to render texture
        lightRenderTexture.clear(sf::Color::Transparent);
        lightRenderTexture.draw(lightShape, &shader);
        lightRenderTexture.display();

        // Draw render texture to window
        sf::Sprite lightSprite(lightRenderTexture.getTexture());
        window.draw(lightSprite);
    }
};


