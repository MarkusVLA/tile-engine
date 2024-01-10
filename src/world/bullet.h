
/**
 * @brief Bullet game object class
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "gameobject.hpp"
#include "../utils/vec.h"
#include "light.h"
#include "lightmap.h"
#include <memory>


class Bullet: public GameObject {
private:
    double speed_;
    int lifetime_;
    Vector2<double> direction_;
    Light bulletLight_;
    std::shared_ptr<LightMap> lMap_;

public:
    Bullet(Vector2<double> pos, sf::Texture& texture, Vector2<double> dir, std::shared_ptr<LightMap> lmap)
        : GameObject(pos, texture), speed_(2000), lifetime_(60), direction_(dir.Normalize()), lMap_(lmap) {
            bulletLight_ = Light(pos, 100, {1.0, 0.4, 0.0});
            lMap_->addLight(&bulletLight_); // Access LightMap through shared_ptr
    }
    
    ~Bullet(){ lMap_->removeLight(&bulletLight_); }

    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;
    bool shouldDestroy(void){ return lifetime_ <= 0; }

    void update(double dt){
        // Update position of bullet by adding moved distance
        pos_ += direction_ * dt * speed_;
        // move light to the bullets position
        bulletLight_.setPosition(pos_);
        lifetime_--;
    }

    void draw(sf::RenderTarget &target) {
        double lineLength = 10.0; // You can adjust this value
        Vector2<double> endPoint = pos_ + direction_ * lineLength;
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(pos_.GetX(), pos_.GetY())),
            sf::Vertex(sf::Vector2f(endPoint.GetX(), endPoint.GetY()))
        };
        line[0].color = sf::Color::White;
        line[1].color = sf::Color::Red;
        target.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // TODO Collision detection and bullet deletion on impact.

};