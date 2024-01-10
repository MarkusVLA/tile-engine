
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
#include "world/bullet.h"
#include "utils/post_processing.h"
#include "utils/sprite_manager.h"
#include <memory>
#include <map>

#define TILESIZE 64
#define FLOORSIZE 256
#define FPS 80

// Global texture storage
std::map<std::string, sf::Texture> textures;

void loadTextures() {
    if (!textures["tile"].loadFromFile("../src/assets/textures/tile.png")) {
        std::cerr << "Failed to load tile texture" << std::endl;
    }
    if (!textures["default"].loadFromFile("../src/assets/textures/default.png")) {
        std::cerr << "Failed to load default texture" << std::endl;
    }

    if (!textures["sheet"].loadFromFile("../src/assets/textures/sheet.png")) {
        std::cerr << "Failed to load default texture" << std::endl;
    }

    if (!textures["floor"].loadFromFile("../src/assets/textures/floor.png")) {
        std::cerr << "Failed to load floor texture" << std::endl;
    }
}

void setUpMap(Map &gameMap) {
    int roomSize = 20;
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

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






    sf::Vector2u windowSize = {800, 800};
    sf::RenderWindow window(sf::VideoMode(windowSize, 8), "Game");
    sf::Vector2u renderTextureSize = {400, 400}; // Size to scale up
    sf::RenderTexture renderTextureMap;
    renderTextureMap.create({renderTextureSize.x, renderTextureSize.y});
    sf::RenderTexture renderTextureLight;
    renderTextureLight.create({renderTextureSize.x, renderTextureSize.y});
    sf::RenderTexture renderTextureUI;
    renderTextureUI.create({renderTextureSize.x, renderTextureSize.y});

    window.setFramerateLimit(FPS);

    // Camera setup
    sf::Rect<float> viewRect(sf::Vector2f(0,0), sf::Vector2f(static_cast<float>(renderTextureSize.x), static_cast<float>(renderTextureSize.y)));
    Camera camera(viewRect);

    loadTextures();
    // Sprite manager
    std::shared_ptr<SpriteManager> sprite_manager = std::make_shared<SpriteManager>();
    // Obstacle setup
    ObstacleManager obstacle_manager;
    // Map setup
    const float movementSpeed = 2.0f;
    Map gameMap;
    setUpMap(gameMap);
    
    Player player(Vector2<double>(), textures["default"]);
    obstacle_manager.buildObstacleMap(gameMap);


    // FLoor setup
    Floor floor(textures["floor"]);

    // Light setup
    Light light2(Vector2<double>(200, 100), 200, {1.0, 0.2, 0.7}, 0.6);
    Light light3(Vector2<double>(400, 300), 200, {0.1, 1.0, 0.6}, 0.6);
    std::shared_ptr<LightMap> light_map = std::make_shared<LightMap>(window);
    light_map->addLight(&light2);
    light_map->addLight(&light3);


    // Render text
    sf::Font font;
    if (!font.loadFromFile("../src/assets/fonts/tuffy.ttf"))
        return EXIT_FAILURE;
    sf::Text text(font, "Time Step", 12);

    // Bullets

    std::vector<std::shared_ptr<Bullet>> bullets;
    volatile int shootCoolDonwn = 0;

    Transforms::PerspectiveShader perspectiveShader;
    Transforms::LightPerspectiveShader lightLayerShader;

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

        double potentialX = player.getX() + (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? movementSpeed : 0.0f));
        double potentialY = player.getY() + (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? -movementSpeed : (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? movementSpeed : 0.0f));
        bool colliding = player.checkCollisionWithMap(gameMap, potentialX, potentialY);
        // BULLETS //
        shootCoolDonwn--;
        bool shooting = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (shooting && bullets.size() < 10 && shootCoolDonwn <= 0) {
            Vector2<double> bulletDir = (mousePosGame - player.GetPos()).Normalize();
            auto newBullet = player.shootBullet(bulletDir, light_map);
            bullets.push_back(newBullet);
            shootCoolDonwn = 15;
        }

        if (!colliding) {
            player.setX(potentialX);
            player.setY(potentialY);
        }

        player.updateSpritePos();
        light_map->castRays(obstacle_manager); 

        camera.setPosition(sf::Vector2f(static_cast<float>(player.getX()), static_cast<float>(player.getY())));
        floor.updateVisibleTiles(camera.getView());
        
        light_map->updateCameraView(Vector2<double>(player.getX(), player.getY()));

        for (auto it = bullets.begin(); it != bullets.end(); ) {
            (*it)->update(1.0 / FPS);
            if ((*it)->shouldDestroy(gameMap)) {
                it = bullets.erase(it);
            } else {
                ++it; 
            }
        }

        text.setString(
            "Player: " + std::to_string(player.getX()) + ", " + std::to_string(player.getY()) 
            +"\nMouse" + std::to_string(mousePosGame.GetX()) + ", " + std::to_string(mousePosGame.GetY())
            +"\nBullets: " + std::to_string(bullets.size())
        );




        //////////// DRAWING /////////////
        // Update the views of render textures to match the camera
        updateRenderTextureView(renderTextureMap, camera);
        updateRenderTextureView(renderTextureLight, camera);
        window.clear();
        renderTextureLight.clear(sf::Color(12, 12, 12));
        renderTextureUI.clear();
        renderTextureMap.clear();
        floor.draw(renderTextureMap);
        gameMap.draw(renderTextureMap);
        player.draw(renderTextureMap);
        renderTextureUI.draw(text);
        light_map->drawLights(renderTextureLight, camera);
        for (auto & bullet: bullets) bullet->draw(renderTextureMap, camera.getPosition());
        sf::Sprite lightSprite(renderTextureLight.getTexture());
        lightSprite.setScale(
            {static_cast<float>(windowSize.x) / renderTextureSize.x, // Scale X
            static_cast<float>(windowSize.y) / renderTextureSize.y}  // Scale Y
        );
        sf::Sprite UI_sprite(renderTextureUI.getTexture());
        UI_sprite.setScale(
            {static_cast<float>(windowSize.x) / renderTextureSize.x, // Scale X
            static_cast<float>(windowSize.y) / renderTextureSize.y}  // Scale Y
        );
        sf::Sprite mapSprite(renderTextureMap.getTexture());
        perspectiveShader.apply(mapSprite, sf::Vector2f(windowSize));
        lightLayerShader.apply(mapSprite, sf::Vector2f(windowSize));
        mapSprite.setScale(
            {static_cast<float>(windowSize.x) / renderTextureSize.x, // Scale X
            static_cast<float>(windowSize.y) / renderTextureSize.y}  // Scale Y
        );
        {   // Draw sprites to window with apropriate shader.
            window.draw(mapSprite, &perspectiveShader.getShader());
            window.draw(lightSprite, sf::RenderStates(sf::BlendMultiply, sf::Transform(), nullptr, &lightLayerShader.getShader()));
            window.draw(UI_sprite, sf::BlendAdd);
            window.display();
        }
    }
    return 0;
}