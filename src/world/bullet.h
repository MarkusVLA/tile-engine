
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
#include <iostream>


class Bullet: public GameObject {
private:
    double speed_;
    int lifetime_;
    Vector2<double> direction_;
    Light bulletLight_;
    std::shared_ptr<LightMap> lMap_;

public:
    Bullet(Vector2<double> pos, sf::Texture& texture, Vector2<double> dir, std::shared_ptr<LightMap> lmap)
        : GameObject(pos, texture), speed_(800), lifetime_(60), direction_(dir.Normalize()), lMap_(lmap) {
            bulletLight_ = Light(pos, 100, {1.0, 0.6, 0.4}, 0.3);
            lMap_->addLight(&bulletLight_); // Access LightMap through shared_ptr
            std::cout << "Created new bullet " <<  this  << " at: " << pos_ << std::endl;
    }
    
    ~Bullet() { 
        lMap_->removeLight(&bulletLight_); 
        std::cout << "Destroyed bullet " <<  this  << std::endl;
    }

    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;


    bool shouldDestroy(Map& map){ 
        if (lifetime_ <= 0 || checkCollisionWithMap(map)){
            return true;
        } else {
            return false;
        }
    }

    void update(double dt){
        pos_ += direction_ * dt * speed_;
        rect_.SetCornerA(pos_);
        rect_.SetCornerB(pos_ + size_);
        bulletLight_.setPosition(pos_);
        updateSpritePos();
        lifetime_--;
    }

    bool checkCollisionWithMap(Map& gameMap) {
    Rect<double> bulletRect = getRect(); 
        for (auto tile : gameMap.getTiles()) {
            Rect<double> tileRect = tile.getRect();  
            if (bulletRect.Intersects(tileRect)) {
                return true; 
            }
        }
        return false;
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

};