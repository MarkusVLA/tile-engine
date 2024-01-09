#include <SFML/Graphics.hpp>
#include "Light.h"
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

    void addLight(Light* light) {
        lights.push_back(light);
    }

    void updateRenderTextureSize(const sf::Vector2u& size) {
        renderTexture_.create({size.x, size.y});
    }

    void drawDebugRays(sf::RenderTarget &target) {
        for (auto& light : lights) {
            std::vector<Vector2<double>> endPoints = light->getEndPoints();

            // Draw each ray as a line
            for (const auto& endPoint : endPoints) {
                sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                line[0].position = sf::Vector2f(light->getPosition().toSF_Vectorf().x, light->getPosition().toSF_Vectorf().y);
                line[0].color = sf::Color::White; // Start point color

                line[1].position = sf::Vector2f(endPoint.toSF_Vectorf().x, endPoint.toSF_Vectorf().y);
                line[1].color = sf::Color::White; // End point color

                target.draw(line);
            }
        }
    }

};
