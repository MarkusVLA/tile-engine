/**
 * @file camera.hpp
 * @author your name (you@domain.com)
 * @brief Game camera
 * @version 0.1
 * @date 2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once
#include <SFML/Graphics.hpp>
#include "world/player.hpp"
#include "world/map.hpp"
#include "utils/vec.h"

class Camera {
private:
    sf::View view_;
    sf::Vector2f targetPosition_;  
    float smoothingFactor_;        

public:

    Camera(const sf::FloatRect& viewRect): view_(viewRect), smoothingFactor_(0.075f) {
    }

    // Getters
    sf::View getView() const { return view_; }
    sf::Vector2f getPosition() const { return view_.getCenter(); }
    sf::Vector2f getSize() const { return view_.getSize(); }

    void move(const sf::Vector2f& delta){ view_.move(delta); }
    void setSize(const sf::Vector2f& size) { view_.setSize(size); }


    void update() {
        // Interpolate between the current position and the target position
        sf::Vector2f currentPosition = view_.getCenter();
        sf::Vector2f newPosition = currentPosition + smoothingFactor_ * (targetPosition_ - currentPosition);
        view_.setCenter(newPosition);
    }

    void setPosition(const sf::Vector2f& position) {
        targetPosition_ = position;  
        update();                    
    }

    sf::Vector2f getTargetPos(void) const {
        return targetPosition_;
    }

    Rect<double> getViewRect(void){
        /* Get Cameras view as a rect in world coordinates */
        sf::Vector2f viewCenter = view_.getCenter();
        sf::Vector2f viewSize = view_.getSize();

        sf::Vector2f A = viewCenter - viewSize * 0.5f;
        sf::Vector2f B = viewCenter + viewSize * 0.5f;
        // construct a Rect
        Rect<double> camViewRect(Vector2<double>(A.x, A.y), Vector2<double>(B.x, B.y));
        return camViewRect;
    }
    
};





