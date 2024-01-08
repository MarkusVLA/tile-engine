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
#include "player.hpp"
#include "map.hpp"

#define TILESIZE 16


class Camera {
private:
    sf::View view_;
    sf::RenderWindow* window_;  // Pointer to the window where the view is applied

public:

    Camera(sf::RenderWindow* window, const sf::FloatRect& viewRect);
    ~Camera();

    // Camera manipulation methods
    void move(const sf::Vector2f& delta);
    void zoom(float factor);
    void rotate(sf::Angle angle);
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    
    // Utility method
    sf::Vector2i worldToPixel(const sf::Vector2f& worldPos) const;

    // Apply the view to the window
    void applyView() const;
};

Camera::Camera(sf::RenderWindow* window, const sf::FloatRect& viewRect)
    : window_(window), view_(viewRect) {
}

Camera::~Camera() {
    // Destructor code if needed
}

void Camera::move(const sf::Vector2f& delta) {
    view_.move(delta);
}

void Camera::zoom(float factor) {
    view_.zoom(factor);
}

void Camera::rotate(sf::Angle angle) {
    view_.rotate(angle);
}

void Camera::setPosition(const sf::Vector2f& position) {
    view_.setCenter(position);
}

void Camera::setSize(const sf::Vector2f& size) {
    view_.setSize(size);
}

sf::Vector2i Camera::worldToPixel(const sf::Vector2f& worldPos) const {

    sf::Vector2f adjustedWorldPos = worldPos * static_cast<float>(TILESIZE);
    // Use the previously defined worldToPixel function
    sf::Vector2f viewSize = view_.getSize();
    sf::Vector2f viewCenter = view_.getCenter();

    // Calculate position relative to the center of the view
    sf::Vector2f relativePos = worldPos - viewCenter;

    // Adjust for the view's size to get the position in view space
    relativePos.x += viewSize.x / 2;
    relativePos.y += viewSize.y / 2;

    // Scale according to the window size
    sf::Vector2f windowSize = view_.getSize();
    sf::Vector2i pixelPos;
    pixelPos.x = static_cast<int>((relativePos.x / viewSize.x) * windowSize.x);
    pixelPos.y = static_cast<int>((relativePos.y / viewSize.y) * windowSize.y);
    return pixelPos;
}

void Camera::applyView() const {
    if (window_) {
        window_->setView(view_);
    }
}




