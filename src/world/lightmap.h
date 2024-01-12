
#pragma once

#include <SFML/Graphics.hpp>
#include "light.h"
#include <vector>
#include <stdexcept>
#include "../utils/rect.h"



class LightMap {
private:
    std::vector<Light*> lights;
    sf::Shader shader_;

public:
    LightMap(sf::RenderWindow& window) {

        sf::Vector2f viewSize(window.getSize().x, window.getSize().y);


        if (!shader_.loadFromFile("../src/shaders/light.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Failed to load shaders.");
        }
    }

    ~LightMap() {
        // Free light map.
        for (auto & light: lights){
            delete light;
        }
    }


    void castRays(ObstacleManager man){
        for (auto light: lights){
            light->castRays(man);
        }
    }

    void addLight(Light* light) {
        lights.push_back(light);
    }

    void removeLight(Light* light) {
        auto it = std::remove(lights.begin(), lights.end(), light);
        if (it != lights.end()) {
            lights.erase(it, lights.end());
        }
    }

   void drawLights(sf::RenderTarget &target, Camera cam) {
        for (auto& light : lights) {
            //light->drawDebug(target, shader_);
            light->fillArea(target, shader_, cam);
        }
   }
};
