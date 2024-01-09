#include <SFML/Graphics.hpp>
#include "light.h"
#include <vector>
#include <stdexcept>
#include "../utils/rect.h"



class LightMap {
private:
    std::vector<Light*> lights;
    sf::RenderTexture renderTexture_;
    sf::Shader shader_;
    sf::View cameraView; // Represents the camera's view

public:
    LightMap(sf::RenderWindow& window) {

        sf::Vector2f viewSize(window.getSize().x, window.getSize().y);
        cameraView.setSize(viewSize);
        cameraView.setCenter(viewSize / 2.0f);
        renderTexture_.setView(cameraView);


        if (!shader_.loadFromFile("../src/shaders/light.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Failed to load shaders.");
        }

        updateRenderTextureSize(window.getSize());
    }

    void updateCameraView(const Vector2<double>& cameraPosition) {
        // Update the camera view's center based on the camera position
        sf::Vector2f cameraPos = cameraView.getCenter();
        cameraView.setCenter(cameraPos);
        renderTexture_.setView(cameraView); // Apply the updated view to the render texture
    }

    void castRays(ObstacleManager man){
        for (auto light: lights){
            light->castRays(man);
        }
    }

    void addLight(Light* light) {
        lights.push_back(light);
    }

    void updateRenderTextureSize(const sf::Vector2u& size) {
        renderTexture_.create({size.x, size.y});
    }

   void drawLights(sf::RenderTarget &target) {
        for (auto& light : lights) {
            //light->drawDebug(target, shader_);
            light->fillArea(target, shader_);
        }
   }
};
