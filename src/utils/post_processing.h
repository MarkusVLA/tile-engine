#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

namespace Transforms {

    class PerspectiveShader {
    private:
        sf::Shader shader;

    public:
        PerspectiveShader() {
            if (!shader.loadFromFile("../src/shaders/perspective_transform.frag", sf::Shader::Fragment)) {
                std::cerr << "Failed to load fragment shader" << std::endl;
            }
        }

        void apply(sf::Sprite& sprite, sf::Vector2f resolution) {
            shader.setUniform("resolution", resolution);
            
        }

        sf::Shader& getShader() {
            return shader;
        }

    };


    
    class LightPerspectiveShader {
    private:
        sf::Shader shader;
        
    public:
        LightPerspectiveShader() {
            if (!shader.loadFromFile("../src/shaders/light_perspective.frag", sf::Shader::Fragment)) {
                std::cerr << "Failed to load fragment shader" << std::endl;
            }
        }

        void apply(sf::Sprite& sprite, sf::Vector2f resolution) {
            shader.setUniform("resolution", resolution);
        }

        sf::Shader& getShader() {
            return shader;
        }

    };

} // namespace Transforms
