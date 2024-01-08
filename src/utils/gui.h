
#pragma once

#include <SFML/Graphics.hpp>

class Slider {
private:
    sf::RectangleShape track;
    sf::CircleShape thumb;

public:
    Slider(float x, float y, float width, float height): track(sf::Vector2f(width, height)), thumb(height / 2) {
        track.setPosition({x, y});
        track.setFillColor(sf::Color::White);

        thumb.setPosition({x, y});
        thumb.setFillColor(sf::Color::Blue);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(track);
        window.draw(thumb);
    }

    bool contains(float x, float y) const {
        return thumb.getGlobalBounds().contains({x, y});
    }

    void moveThumb(float x) {
        if (x < track.getPosition().x) {
            thumb.setPosition({track.getPosition().x, thumb.getPosition().y});
        } else if (x > track.getPosition().x + track.getSize().x - thumb.getRadius()) {
            thumb.setPosition({track.getPosition().x + track.getSize().x - thumb.getRadius(), thumb.getPosition().y});
        } else {
            thumb.setPosition({x, thumb.getPosition().y});
        }
    }

    float getValue() const {
        return (thumb.getPosition().x - track.getPosition().x) / (track.getSize().x - thumb.getRadius());
    }
};


#include <SFML/Graphics.hpp>

class Button {
private:
    sf::RectangleShape buttonShape;
public:
    Button(float x, float y, float width, float height): buttonShape({width, height}) {
        buttonShape.setPosition({x, y});
        buttonShape.setFillColor(sf::Color::White);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(buttonShape);
    }

    bool contains(float x, float y) const {
        return buttonShape.getGlobalBounds().contains({x, y});
    }

};