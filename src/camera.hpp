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
    sf::RenderWindow* window_;  // Pointer to the window where the view is applied

public:

    Camera(sf::RenderWindow* window, const sf::FloatRect& viewRect);
    ~Camera();

    // Getters
    sf::View getView() const { return view_; }
    sf::Vector2f getPosition() const { return view_.getCenter(); }
    sf::Vector2f getSize() const { return view_.getSize(); }

    // Camera manipulation methods
    void move(const sf::Vector2f& delta);
    void zoom(float factor);
    void rotate(sf::Angle angle);
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
};




Camera::Camera(sf::RenderWindow* window, const sf::FloatRect& viewRect)
    : window_(window), view_(viewRect) {
}

Camera::~Camera() { }

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





