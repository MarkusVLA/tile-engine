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

int main() {
    sf::Vector2u windowSize = {400, 400};
    sf::RenderWindow window(sf::VideoMode(windowSize, 8), "SFML Window");
    window.setFramerateLimit(60); // frame rate

    sf::Texture playerTexture;

    // Load the player texture
    if (!playerTexture.loadFromFile("../src/assets/default.png")) {
        std::cerr << "Cannot find file" << std::endl;
        return 1;
    }

    Player player(200, 200, playerTexture); // Create player at (200, 200)
    const float movementSpeed = 5.0f; // Pixels per second

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Reset movement each frame
        float movementX = 0.0f, movementY = 0.0f;

        // Update player position based on key presses
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movementY = -movementSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movementX = -movementSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movementY = movementSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movementX = movementSpeed;

        player.setX(player.getX() + movementX);
        player.setY(player.getY() + movementY);
        player.updateSpritePos(); 

        window.clear();
        player.draw(window);
        window.display();
    }

    return 0;
}