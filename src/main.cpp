
/**
 * @file main.cpp
 * @brief 
 */

/*

#include <SFML/Graphics.hpp>
#include "world/player.hpp"
#include "world/map.hpp"
#include "camera.hpp"
#include "utils/vec.h"
#include "world/tile.hpp"
#include "world/light.h"
#include "world/lightmap.h"
#include "world/floor.h"
#include "world/cursor.h"
#include "utils/rand.h"
#include "world/bullet.h"
#include "utils/post_processing.h"
#include "utils/sprite_manager.h"
#include <memory>
#include <map>

#include "game.hpp"

#define TILESIZE 64
#define FLOORSIZE 256
#define FPS 80

void updateRenderTextureView(sf::RenderTexture& renderTexture, const Camera& camera) {
    sf::View view = camera.getView(); // Assuming your Camera class has a getView method
    renderTexture.setView(view);
}


int main() {

    sf::Vector2u windowSize = {800, 800};
    sf::RenderWindow window(sf::VideoMode(windowSize, 8), "Game");
    window.setMouseCursorVisible(false);
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

    // Sprite manager
    std::shared_ptr<SpriteManager> sprite_manager = std::make_shared<SpriteManager>();

    // Obstacle setup
    ObstacleManager obstacle_manager;
    // Map setup
    const float movementSpeed = 2.0f;
    // Game map setup
    Map gameMap(sprite_manager);
    
    Player player(Vector2<double>(-100, -100), sprite_manager);
    obstacle_manager.buildObstacleMap(gameMap);


    // FLoor setup
    Floor floor(sprite_manager);

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


    // Mouse cursor.
    Cursor cursor(sprite_manager);

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
        
        cursor.setPos(mousePosGame);

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

        text.setPosition(camera.getPosition() - sf::Vector2f(renderTextureSize.x / 2, renderTextureSize.y / 2));


        //////////// DRAWING /////////////
        // Update the views of render textures to match the camera
        updateRenderTextureView(renderTextureMap, camera);
        updateRenderTextureView(renderTextureLight, camera);
        updateRenderTextureView(renderTextureUI, camera);
        

        window.clear();
        renderTextureLight.clear(sf::Color(12, 12, 12));
        renderTextureMap.clear();
        renderTextureUI.clear();

        floor.draw(renderTextureMap);
        gameMap.draw(renderTextureMap);
        player.draw(renderTextureMap);
        renderTextureUI.draw(text);
        cursor.draw(renderTextureUI);

        renderTextureUI.display();

        light_map->drawLights(renderTextureLight, camera);
        for (auto & bullet: bullets) bullet->draw(renderTextureMap, camera.getPosition());
        sf::Sprite lightSprite(renderTextureLight.getTexture());
        lightSprite.setScale(
            {static_cast<float>(windowSize.x) / renderTextureSize.x, // Scale X
            static_cast<float>(windowSize.y) / renderTextureSize.y}  // Scale Y
        );
        
        sf::Sprite mapSprite(renderTextureMap.getTexture());
        mapSprite.setScale(
            {static_cast<float>(windowSize.x) / renderTextureSize.x, // Scale X
            static_cast<float>(windowSize.y) / renderTextureSize.y}  // Scale Y
        );

        sf::Sprite renderTextureUISprite(renderTextureUI.getTexture());
        renderTextureUISprite.setScale(
            {static_cast<float>(windowSize.x) / renderTextureSize.x, // Scale X
            static_cast<float>(windowSize.y) / renderTextureSize.y}  // Scale Y
        );


        perspectiveShader.apply(mapSprite, sf::Vector2f(windowSize));
        lightLayerShader.apply(lightSprite, sf::Vector2f(windowSize));
        
        
        {   // Draw sprites to window with apropriate shader.
            window.draw(mapSprite, &perspectiveShader.getShader());
            window.draw(lightSprite, sf::RenderStates(sf::BlendMultiply, sf::Transform(), nullptr, &lightLayerShader.getShader()));
            window.draw(renderTextureUISprite, sf::BlendAdd);
            window.display();
        }
    }
    return 0;
}

*/
/**/

#include "game.hpp"
#include <iostream>

int main() {
    std::cout << "Program started" << std::endl;
    std::cout.flush(); // Ensure the output is flushed immediately
    Game game;
    try {
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    return 0;
}
