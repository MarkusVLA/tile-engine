#pragma once
#include <SFML/Graphics.hpp>
#include "utils/vec.h"

class Obstacle {
private:
    Vector2<double> start;
    Vector2<double> end;

public:
    Obstacle(Vector2<double> start, Vector2<double> end) : start(start), end(end) {}

    Vector2<double> getStart() const { return start; }
    Vector2<double> getEnd() const { return end; }

    void draw(sf::RenderWindow& window) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(static_cast<float>(start.GetX()), static_cast<float>(start.GetY())), sf::Color::White),
            sf::Vertex(sf::Vector2f(static_cast<float>(end.GetX()), static_cast<float>(end.GetY())), sf::Color::White)
        };
        
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

};
