/**
 * @file main.cpp
 * @brief 
 */

#include <SFML/Graphics.hpp>
#include "world/player.hpp"
#include "world/map.hpp"
#include "camera.hpp"
#include "utils/vec.h"
#include "world/tile.hpp"
#include "world/light.h"
#include "world/lightmap.h"

#define TILESIZE 16

void setUpMap(Map &gameMap){
    sf::Texture tex;
    tex.loadFromFile("../src/assets/default.png");
    gameMap.addTile(Tile(Vector2<double>(), tex));
}

void updateRenderTextureView(sf::RenderTexture& renderTexture, const Camera& camera) {
    sf::View view = camera.getView(); // Assuming your Camera class has a getView method
    renderTexture.setView(view);
}

int main() {
    sf::Vector2u windowSize = {800, 600};
    sf::RenderWindow window(sf::VideoMode(windowSize, 8), "Game");

    // Create render textures for different layers
    sf::RenderTexture renderTextureMap;
    renderTextureMap.create({windowSize.x, windowSize.y});
    sf::RenderTexture renderTextureLight;
    renderTextureLight.create({windowSize.x, windowSize.y});

    window.setFramerateLimit(60); // Set frame rate limit

    // Camera setup
    sf::Rect<float> viewRect(sf::Vector2f(0,0), sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    Camera camera(viewRect);

    // Player setup
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../src/assets/default.png")) {
        std::cerr << "Cannot find player texture file" << std::endl;
        return 1;
    }
    Player player(Vector2<double>(200,200), playerTexture);

    // Light setup
    Light light(Vector2<double>(player.getX(), player.getY()));
    LightMap light_map(window); // Create a LightMap instance
    light_map.addLight(&light); // Add the light to the LightMap

    // Obstacle setup
    ObstacleManager obstacle_manager;
    Obstacle obstacle1(Vector2<double>(100, 100), Vector2<double>(200, 100));
    obstacle_manager.addObstacle(obstacle1);

    // Map setup
    const float movementSpeed = 5.0f;
    Map gameMap;
    setUpMap(gameMap);
    obstacle_manager.buildObstacleMap(gameMap);

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Player movement
        double potentialX = player.getX() + (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? movementSpeed : 0.0f));
        double potentialY = player.getY() + (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? movementSpeed : 0.0f));
        player.setX(potentialX);
        player.setY(potentialY);
        player.updateSpritePos();

        // Update light position to track the player
        light.setPosition(Vector2<double>(player.getX(), player.getY()));
        light.castRays(obstacle_manager);

        // Update camera position
        camera.setPosition(sf::Vector2f(static_cast<float>(player.getX()), static_cast<float>(player.getY())));
        // camera.applyView();

        // Update the views of render textures to match the camera
        updateRenderTextureView(renderTextureMap, camera);
        updateRenderTextureView(renderTextureLight, camera);


        // Render the game map to its texture
        renderTextureMap.clear();
        gameMap.draw(renderTextureMap);
        player.draw(renderTextureMap);
        obstacle_manager.draw(renderTextureMap);
        renderTextureMap.display();

        // Render the lighting to its texture
        /*
        renderTextureLight.clear(sf::Color::Transparent);
        light_map.updateCameraView(Vector2<double>(player.getX(), player.getY()));
        light_map.drawDebugRays(renderTextureLight);
        renderTextureLight.display();
        */

        // Combine the rendered textures and draw them to the window
        window.clear();
        sf::Sprite mapSprite(renderTextureMap.getTexture());
        window.draw(mapSprite); // Draw the game map

        sf::Sprite lightSprite(renderTextureLight.getTexture());
        window.draw(lightSprite, sf::BlendAdd); // Draw the lighting with blending
        

       window.display();
    }

    return 0;
}