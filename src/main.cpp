
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
#include "world/floor.h"
#include "utils/rand.h"
#include <map>

#define TILESIZE 64
#define FLOORSIZE 256


// Global texture storage
std::map<std::string, sf::Texture> textures;

void loadTextures() {
    if (!textures["tile"].loadFromFile("../src/assets/tile.png")) {
        std::cerr << "Failed to load tile texture" << std::endl;
    }
    if (!textures["default"].loadFromFile("../src/assets/default.png")) {
        std::cerr << "Failed to load default texture" << std::endl;
    }

    if (!textures["floor1"].loadFromFile("../src/assets/floor1.png")) {
        std::cerr << "Failed to load light texture" << std::endl;
    }

    if (!textures["floor2"].loadFromFile("../src/assets/floor2.png")) {
        std::cerr << "Failed to load light texture" << std::endl;
    }

    if (!textures["floor3"].loadFromFile("../src/assets/floor3.png")) {
        std::cerr << "Failed to load light texture" << std::endl;
    }
}

void setUpMap(Map &gameMap) {
    int roomSize = 20; // Change this to the size of your room
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count(); // Use current time as seed

    // Create the room
    for (int y = 0; y < roomSize; y++) {
        for (int x = 0; x < roomSize; x++) {
            // Add some random tiles
            if (Random::GetRandomDoubleUniform(0.0, 1.0, seed) < 0.1) { // 10% chance to add a tile
                gameMap.addTile(Tile(Vector2<double>(x * TILESIZE, y * TILESIZE), textures["tile"]));
            }
        }
    }
}

void updateRenderTextureView(sf::RenderTexture& renderTexture, const Camera& camera) {
    sf::View view = camera.getView(); // Assuming your Camera class has a getView method
    renderTexture.setView(view);
}


int main() {


    sf::Vector2u windowSize = {1600, 900};
    sf::RenderWindow window(sf::VideoMode(windowSize, 8), "Game");
    sf::Vector2u renderTextureSize = {1600, 900};

    // Create render textures for different layers with the smaller size
    sf::RenderTexture renderTextureMap;
    renderTextureMap.create({renderTextureSize.x, renderTextureSize.y});
    sf::RenderTexture renderTextureLight;
    renderTextureLight.create({renderTextureSize.x, renderTextureSize.y});

    window.setFramerateLimit(60); // Set frame rate limit

    // Camera setup
    sf::Rect<float> viewRect(sf::Vector2f(0,0), sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    Camera camera(viewRect);

    loadTextures();

    // Obstacle setup
    ObstacleManager obstacle_manager;
    Obstacle obstacle1(Vector2<double>(100, 100), Vector2<double>(200, 100));
    obstacle_manager.addObstacle(obstacle1);

    // Map setup
    const float movementSpeed = 5.0f;
    Map gameMap;
    setUpMap(gameMap);
    
    Player player(Vector2<double>(), textures["default"]);
    obstacle_manager.buildObstacleMap(gameMap);


    // FLoor setup
    Floor floor(textures["floor3"]);


    Light light(Vector2<double>(player.getX(), player.getY()));
    Light light2(Vector2<double>(200, 200));
    LightMap light_map(window);
    light_map.addLight(&light);
    light_map.addLight(&light2);


    // Render text
    sf::Font font;
    if (!font.loadFromFile("../src/assets/fonts/tuffy.ttf"))
        return EXIT_FAILURE;
    sf::Text text(font, "Time Step", 20);



    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);

        Vector2<double> mousePosGame = Vector2<double>(
            (static_cast<double>(mousePosWindow.x) * renderTextureSize.x) / windowSize.x - renderTextureSize.x / 2.0 + camera.getPosition().x,
            (static_cast<double>(mousePosWindow.y) * renderTextureSize.y) / windowSize.y - renderTextureSize.y / 2.0 + camera.getPosition().y
        );



        // Calculate potential new position
        double potentialX = player.getX() + (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? movementSpeed : 0.0f));
        double potentialY = player.getY() + (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? movementSpeed : 0.0f));

        // Check for collision at the potential new position
        bool colliding = player.checkCollisionWithMap(gameMap, potentialX, potentialY);
        
        if (!colliding) {
            // Update player position only if not colliding
            player.setX(potentialX);
            player.setY(potentialY);
        }
        
        player.updateSpritePos();

        // Update light position to track the player
        light.setPosition(Vector2<double>(player.getX(), player.getY()));
        light2.setPosition(mousePosGame); // Second light tracks mouse.
        light_map.castRays(obstacle_manager); 
    

        // Update camera position
        camera.setPosition(sf::Vector2f(static_cast<float>(player.getX()), static_cast<float>(player.getY())));
        
        floor.updateVisibleTiles(camera.getView());

        // Update the views of render textures to match the camera
        updateRenderTextureView(renderTextureMap, camera);
        updateRenderTextureView(renderTextureLight, camera);

        // Render the game map to its texture
        renderTextureMap.clear();
        floor.draw(renderTextureMap);
        gameMap.draw(renderTextureMap);
        player.draw(renderTextureMap);
        renderTextureMap.display();

        // Render the lighting to its texture

      
        renderTextureLight.clear(sf::Color(20, 20, 30));
        light_map.updateCameraView(Vector2<double>(player.getX(), player.getY()));
        light_map.drawLights(renderTextureLight, camera);
        renderTextureLight.display();
       

        window.clear();

        sf::Sprite mapSprite(renderTextureMap.getTexture());
        mapSprite.setScale(
            {static_cast<float>(windowSize.x) / renderTextureSize.x, // Scale X
            static_cast<float>(windowSize.y) / renderTextureSize.y}  // Scale Y
        );
        window.draw(mapSprite); // Draw the scaled game map

        
        sf::Sprite lightSprite(renderTextureLight.getTexture());
        lightSprite.setScale(
            {static_cast<float>(windowSize.x) / renderTextureSize.x, // Scale X
            static_cast<float>(windowSize.y) / renderTextureSize.y}  // Scale Y
        );

        // Debug text
        text.setString(
            "Player: " + std::to_string(player.getX()) + ", " + std::to_string(player.getY()) 
            + "\nMouse" + std::to_string(mousePosGame.GetX()) + ", " + std::to_string(mousePosGame.GetY())
        );

        window.draw(lightSprite, sf::BlendMultiply); // Draw the scaled lighting with blending
        window.draw(text, sf::BlendAdd);
        window.display();
    }

    return 0;
}