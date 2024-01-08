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
#include "player.hpp"
#include "map.hpp"
#include "camera.hpp"

#define TILESIZE 16

void setUpMap(Map &gameMap){

    sf::Texture tex;
    tex.loadFromFile("../src/assets/default.png");
    gameMap.addTile(Tile(0, 0, tex));
    gameMap.addTile(Tile(10, 10, tex));
    gameMap.addTile(Tile(60, 60, tex));
}



int main() {
    sf::Vector2u windowSize = {800, 600};
    sf::RenderWindow window(sf::VideoMode(windowSize, 8), "SFML Window");
    window.setFramerateLimit(60); // frame rate

    sf::Rect<float> viewRect(sf::Vector2f(0,0), sf::Vector2f(windowSize.x, windowSize.y));
    Camera camera(&window, viewRect);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../src/assets/default.png")) {
        std::cerr << "Cannot find file" << std::endl;
        return 1;
    }

    Player player(200, 200, playerTexture);
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

        float potentialX = player.getX() + (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? movementSpeed : 0.0f));
        float potentialY = player.getY() + (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? movementSpeed : 0.0f));
    

        if (!player.checkCollision(potentialX, potentialY, gameMap.getTiles())) {
            player.setX(potentialX);
            player.setY(potentialY);
            player.updateSpritePos();
        }

        camera.setPosition(sf::Vector2f(player.getX(), player.getY()));
        camera.applyView();

        window.clear();
        player.draw(window);
        gameMap.draw(window);
        window.display();
    }

    return 0;
}
