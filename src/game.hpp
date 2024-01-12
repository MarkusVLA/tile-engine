#pragma once

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
#include "world/enemies.h"
#include "utils/post_processing.h"
#include "utils/sprite_manager.h"
#include <memory>
#include <map>


struct ScreenParams {
    std::string name = "Tile Game";
    sf::Vector2u windowSize = {1000, 1000};
    sf::Vector2u renderTextureSize = {400, 400};
    const int FPS = 120;
    ScreenParams(){}
};

struct RenderTargets {

    sf::RenderTexture renderTextureMap;
    sf::RenderTexture renderTextureLight;
    sf::RenderTexture renderTextureUI;
    RenderTargets(ScreenParams params) {
        if(!renderTextureMap.create({params.renderTextureSize.x, params.renderTextureSize.y})){
            std::cerr << "Unable to create render texture" << std::endl;
        } if(!renderTextureLight.create({params.renderTextureSize.x, params.renderTextureSize.y})){
            std::cerr << "Unable to create render texture" << std::endl;
        } if(!renderTextureUI.create({params.renderTextureSize.x, params.renderTextureSize.y})){
            std::cerr << "Unable to create render texture" << std::endl;
        }
    }

};

struct Shaders {
    Transforms::PerspectiveShader perspectiveShader;
    Transforms::LightPerspectiveShader lightLayerShader;
    Shaders(){
    }
};


// Move this to player class
volatile int shootCoolDonwn = 0;


class Game {

private:

    ScreenParams params_;
    RenderTargets targets_;
    Shaders shaders_;
    sf::RenderWindow window_;

    Camera camera_;
    ObstacleManager obstacle_manager_;
    std::shared_ptr<SpriteManager> sprite_manager_;
    std::shared_ptr<LightMap> light_map_;
    std::vector<std::shared_ptr<Bullet>> bullets;
    sf::Font font;
    sf::Text text_;

    Floor floor_;
    Player player_;
    std::vector<Enemy> enemies_;
    Map gameMap_; 
    Cursor cursor_;
    
public:

   Game()
        : params_(), targets_(params_), shaders_(), window_(),
        sprite_manager_(std::make_shared<SpriteManager>()),
        light_map_(std::make_shared<LightMap>(window_)),
        camera_(sf::Rect<float>(sf::Vector2f(0,0), sf::Vector2f(
            static_cast<float>(params_.renderTextureSize.x),
            static_cast<float>(params_.renderTextureSize.y)))),
        text_(font, "", 10 ),
        floor_(sprite_manager_),
        player_(Vector2<double>(-30, -30), sprite_manager_),
        gameMap_(sprite_manager_),
        cursor_(sprite_manager_)

    {
        std::cout << "Starting game" << std::endl;
        window_.create(sf::VideoMode(params_.windowSize, 8), params_.name);
        window_.setFramerateLimit(params_.FPS);
        window_.setMouseCursorVisible(false);
        if (!font.loadFromFile("../src/assets/fonts/tuffy.ttf")) {
            std::cerr << "Unable to load font" << std::endl;
        }
        obstacle_manager_.buildObstacleMap(gameMap_);
        enemies_.emplace_back(Vector2<double> (300, 300), sprite_manager_);

    }

    void updateRenderTargetView(RenderTargets& targets, const Camera& camera) {
        sf::View view = camera.getView();
        targets.renderTextureMap.setView(view);
        targets.renderTextureLight.setView(view);
        targets.renderTextureUI.setView(view);
    }

    Vector2<double> getMousePos(){
        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window_);
        Vector2<double> mousePosGame = Vector2<double>(
                (static_cast<double>(mousePosWindow.x) * params_.renderTextureSize.x) / params_.windowSize.x - params_.renderTextureSize.x / 2.0 + camera_.getPosition().x,
                (static_cast<double>(mousePosWindow.y) * params_.renderTextureSize.y) / params_.windowSize.y - params_.renderTextureSize.y / 2.0 + camera_.getPosition().y
            );
        return mousePosGame;
    }


    void updateGameState(void){

        Vector2<double> mousePosGame = getMousePos();
        cursor_.setPos(mousePosGame);

        double moveX = (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? -1 : (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1 : 0.0f));
        double moveY = (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? -1 : (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1 : 0.0f));
        player_.move(Vector2<double>(moveX, moveY), gameMap_);
        
        // test enemy movement
        for (auto &enemy: enemies_){
            enemy.setTarget(player_.getPosition());
            enemy.follow(gameMap_); // follow the player
        
        }
        
        // BULLETS //
        shootCoolDonwn--;
        bool shooting = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (shooting && bullets.size() < 10 && shootCoolDonwn <= 0) {
            Vector2<double> bulletDir = (mousePosGame - player_.getPosition()).Normalize();
            auto newBullet = player_.shootBullet(bulletDir, light_map_);
            bullets.push_back(newBullet);
            shootCoolDonwn = 15;
        }

        obstacle_manager_.buildObstacleMap(gameMap_); // Build map again so that only tiles in camera view are inclued
        light_map_->castRays(obstacle_manager_); 
        camera_.setPosition(sf::Vector2f(static_cast<float>(player_.getX()), static_cast<float>(player_.getY())));
        floor_.updateVisibleTiles(camera_.getView());
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            (*it)->update(1.0 / params_.FPS);
            if ((*it)->shouldDestroy(gameMap_)) {
                it = bullets.erase(it);
            } else {
                ++it; 
            }
        }

        gameMap_.setTileVisibility(camera_);

        text_.setString(
            "Player: " + std::to_string(player_.getX()) + ", " + std::to_string(player_.getY())
            +"\nTiles: " + std::to_string(gameMap_.getTiles().size())
            +"\nBullets: " + std::to_string(bullets.size())
        );

        text_.setPosition(camera_.getPosition() - sf::Vector2f(params_.renderTextureSize.x / 2, params_.renderTextureSize.y / 2));

    }

    void drawGameState(void){

        updateRenderTargetView(targets_, camera_);
        window_.clear();
        targets_.renderTextureLight.clear(sf::Color(60, 60, 80));
        targets_.renderTextureMap.clear();
        targets_.renderTextureUI.clear();
        floor_.draw(targets_.renderTextureMap);
        gameMap_.draw(targets_.renderTextureMap);
        player_.draw(targets_.renderTextureMap);
        targets_.renderTextureUI.draw(text_);
        cursor_.draw(targets_.renderTextureUI);
        targets_.renderTextureUI.display();
        light_map_->drawLights(targets_.renderTextureLight, camera_);

        for (auto & enemy: enemies_){
            enemy.draw(targets_.renderTextureMap);
        }

        for (auto & bullet: bullets) bullet->draw(targets_.renderTextureMap, camera_.getPosition());
        sf::Sprite lightSprite(targets_.renderTextureLight.getTexture());
        lightSprite.setScale(
            {static_cast<float>(params_.windowSize.x) / params_.renderTextureSize.x, // Scale X
            static_cast<float>(params_.windowSize.y) / params_.renderTextureSize.y}  // Scale Y
        );
        
        sf::Sprite mapSprite(targets_.renderTextureMap.getTexture());
        mapSprite.setScale(
            {static_cast<float>(params_.windowSize.x) / params_.renderTextureSize.x, // Scale X
            static_cast<float>(params_.windowSize.y) / params_.renderTextureSize.y}  // Scale Y
        );

        sf::Sprite renderTextureUISprite(targets_.renderTextureUI.getTexture());
        renderTextureUISprite.setScale(
            {static_cast<float>(params_.windowSize.x) / params_.renderTextureSize.x, // Scale X
            static_cast<float>(params_.windowSize.y) / params_.renderTextureSize.y}  // Scale Y
        );


        shaders_.perspectiveShader.apply(mapSprite, sf::Vector2f(params_.windowSize));
        shaders_.lightLayerShader.apply(lightSprite, sf::Vector2f(params_.windowSize));
        
        {
            window_.draw(mapSprite, &shaders_.perspectiveShader.getShader());
            window_.draw(lightSprite, sf::RenderStates(sf::BlendMultiply, sf::Transform(), nullptr, &shaders_.lightLayerShader.getShader()));
            window_.draw(renderTextureUISprite, sf::BlendAdd);
            window_.display();
        }
    }


    void run() {
        
        Light light1(Vector2<double>(0,0), 200, {0.8,5,0}, 0.3);
        Light light2(Vector2<double>(190, 100), 200, {1.0, 0.2, 0.7}, 0.6);
        Light light3(Vector2<double>(-20, 220), 200, {0.1, 1.0, 0.6}, 0.6);
        light_map_->addLight(&light2);
        light_map_->addLight(&light3);
        light_map_->addLight(&light1);

        
        while (window_.isOpen()) {
            sf::Event event;
            while (window_.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window_.close();
                } if (event.type == sf::Event::Resized){

                }
            }

            updateGameState();
            drawGameState();            
        
        }
        
    }

};
