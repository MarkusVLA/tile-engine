#pragma once

#include <SFML/Graphics.hpp>
#include "../utils/vec.h"
#include "../utils/obstacle_manager.h" 
#include "../utils/ray.h"
#include "../utils/segment.h"
#include "../camera.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

class Light {
private:
    Vector2<double> position;
    int numRays;
    double radius;
    std::vector<Segment<double>> raySegments_;  
    float angleIncrement;  
    sf::Vector3f color_;
    float gradientWidth_;


public:

    Light() {} // Default

    Light(Vector2<double> position): position(position), numRays(720), color_({1.0, 1.0, 0.75}), gradientWidth_(0.5) {
        angleIncrement = 360.0f / static_cast<float>(numRays);
    }
    Light(Vector2<double> position, int numRays, sf::Vector3f color, float width): position(position), numRays(numRays), color_(color), gradientWidth_(width) {
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
        raySegments_.clear();

        float angleIncrement = 360.0f / static_cast<float>(numRays);

        for (int i = 0; i < numRays; ++i) {
            float angle = static_cast<float>(i) * angleIncrement * static_cast<float>(M_PI) / 180.0f;
            Ray ray(position, angle);
            Vector2<double> endPoint = ray.cast(obstacles);
            raySegments_.push_back(Segment(position, endPoint));
        }
    }

    const std::vector<Segment<double>>& getEndPoints() const { return raySegments_; }
    
    void drawDebugRays(sf::RenderTarget& target, sf::Shader& shader) {
        for (const auto &segment : raySegments_) {
            sf::Vertex line[] = {
                sf::Vertex(segment.GetBegin().toSF_Vectorf(), sf::Color::White),
                sf::Vertex(segment.GetEnd().toSF_Vectorf(), sf::Color::White)
            };
            target.draw(line, 2, sf::PrimitiveType::Lines, &shader);
        }
    }

    void fillArea(sf::RenderTarget& target, sf::Shader &shader, Camera &cam){

        shader.setUniform("lightPos", position.toSF_Vectorf());
        shader.setUniform("renderTargetRes", sf::Vector2f({static_cast<float>(target.getSize().x), static_cast<float>(target.getSize().y)}));
        shader.setUniform("cameraPos", cam.getPosition());
        shader.setUniform("lightColor", color_);
        shader.setUniform("gradientWidth", gradientWidth_);

        for (int i = 0; i < raySegments_.size(); i++){
            const auto& seg = raySegments_[i];
            sf::ConvexShape Tria(3);

            if (i == raySegments_.size() - 1) {
                Tria.setPoint(0, sf::Vector2f(raySegments_[raySegments_.size() - 1].GetEnd().GetX(), raySegments_[raySegments_.size() - 1].GetEnd().GetY()));
                Tria.setPoint(1, sf::Vector2f(raySegments_[raySegments_.size() - 1].GetBegin().GetX(), raySegments_[raySegments_.size() - 1].GetBegin().GetY()));
                Tria.setPoint(2, sf::Vector2f(raySegments_[0].GetEnd().GetX(), raySegments_[0].GetEnd().GetY()));
            } else {
                Tria.setPoint(0, sf::Vector2f(raySegments_[i].GetEnd().GetX(), raySegments_[i].GetEnd().GetY()));
                Tria.setPoint(1, sf::Vector2f(raySegments_[i].GetBegin().GetX(), raySegments_[i].GetBegin().GetY()));
                Tria.setPoint(2, sf::Vector2f(raySegments_[i + 1].GetEnd().GetX(), raySegments_[i + 1].GetEnd().GetY()));
            }
            Tria.setFillColor(sf::Color(255, 255, 255, 220));
            target.draw(Tria, sf::RenderStates(sf::BlendAdd, sf::Transform(), nullptr, &shader));
        }
    }


    friend std::ostream& operator<<(std::ostream& os, const Light& light) {
        os << "Light: " << light.position << std::endl;
        return os;
    }

};


