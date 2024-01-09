// Fragment Shader
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform vec2 lightPos; // Light position in SFML coordinates
uniform vec2 renderTargetRes;

void main() {

    // Convert fragment position and light position to normalized space
    vec2 uv = gl_FragCoord.xy / renderTargetRes;
    vec2 light = (lightPos / renderTargetRes / 1000000) + 0.5;
    light.y = 1.0 - light.y; 

    float distance = length(uv - light);

    float gradientWidth = 0.3;
    float alpha = 1.0 - smoothstep(0.0, gradientWidth, distance);

    
    vec3 lightColor = vec3(1.0, 1.0, 0.5); // White light
    FragColor = vec4(lightColor, alpha);
}
