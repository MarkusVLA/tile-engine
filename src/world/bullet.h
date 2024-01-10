
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
    sf::Shader shader_;

public:
    Bullet(Vector2<double> pos, sf::Texture& texture, Vector2<double> dir, std::shared_ptr<LightMap> lmap)
        : GameObject(pos, texture), speed_(600), lifetime_(60), direction_(dir.Normalize()), lMap_(lmap) {
            bulletLight_ = Light(pos, 100, {1.0, 0.6, 0.4}, 0.3);
            lMap_->addLight(&bulletLight_); // Access LightMap through shared_ptr

            if (!shader_.loadFromFile("../src/shaders/bullet.frag", sf::Shader::Fragment)) {
                throw std::runtime_error("Failed to load bullet shader.");
            }

            
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

    void draw(sf::RenderTarget &target, sf::Vector2f camPos) {
        shader_.setUniform("bulletPos", sf::Vector2f(pos_.GetX(), pos_.GetY()));
        shader_.setUniform("cameraPos", camPos);
        shader_.setUniform("renderTargetRes", sf::Vector2f(target.getSize().x, target.getSize().y));
        shader_.setUniform("bulletDirection", direction_.toSF_Vectorf());
        shader_.setUniform("bulletTexture", texture_);
        sf::CircleShape bulletSurface(30); 
        bulletSurface.setOrigin({30, 30}); 
        bulletSurface.setPosition(pos_.toSF_Vectorf());
        target.draw(bulletSurface, &shader_);
    }
};