/**
 * @file main.cpp
 * @author Markus Lång
 * @brief 
 * @version 0.1
 * @date 2023-11-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <SFML/Graphics.hpp>
#include "world/player.hpp"
#include "world/map.hpp"
#include "camera.hpp"
#include "utils/vec.h"
#include "world/tile.hpp"
#include "world/light.h"

#define TILESIZE 16

void setUpMap(Map &gameMap){

    sf::Texture tex;
    tex.loadFromFile("../src/assets/default.png");
    gameMap.addTile(Tile(Vector2<double>(), tex));
}


int main() {
    sf::Vector2u windowSize = {800, 600};
    sf::RenderWindow window(sf::VideoMode(windowSize, 8), "Game");
    window.setFramerateLimit(60); // frame rate

    sf::Rect<float> viewRect(sf::Vector2f(0,0), sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    Camera camera(&window, viewRect);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../src/assets/default.png")) {
        std::cerr << "Cannot find file" << std::endl;
        return 1;
    }

    Player player(Vector2<double>(200,200), playerTexture);
    // Create a Light object at the player's initial position
    Light light(Vector2<double>(player.getX(), player.getY()));
    // Set up the ObstacleManager if required by Light
    ObstacleManager obstacleManager;

    Obstacle obstacle1(Vector2<double>(100, 100), Vector2<double>(200, 100));
    obstacleManager.addObstacle(obstacle1);

    const float movementSpeed = 5.0f;

    Map gameMap;
    setUpMap(gameMap);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        double potentialX = player.getX() + (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? movementSpeed : 0.0f));
        double potentialY = player.getY() + (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? movementSpeed : 0.0f));
        

        player.setX(potentialX);
        player.setY(potentialY);
        player.updateSpritePos();

        // Update light position to track the player
        light.setPosition(Vector2<double>(player.getX(), player.getY()));

        // Update the light's rays based on the current scene
        light.castRays(obstacleManager);
       

        camera.setPosition(sf::Vector2f(static_cast<float>(player.getX()), static_cast<float>(player.getY())));

        camera.applyView();

        window.clear();
        player.draw(window);
        gameMap.draw(window);

        // Draw the light
        light.draw(window);
        obstacleManager.draw(window);

        window.display();
    }

    return 0;
}
