// Fragment Shader
#version 120

varying vec2 TexCoord;

uniform vec2 lightPos; // Light position in SFML coordinates
uniform vec2 cameraPos; // Position of view.
uniform vec2 renderTargetRes;
uniform vec3 lightColor;


void main() {

    vec2 uv = gl_FragCoord.xy / renderTargetRes;
    // Convert SFML coordinates to OpenGL
    vec2 screepos = (cameraPos + renderTargetRes - lightPos) / renderTargetRes;
    // vec2 light = vec2(0.5);
    vec2 light = screepos - vec2(0.5);

    light.x = 1.0 - light.x; 
    float distance = length(uv - light);

    float gradientWidth = 0.4;
    float t = clamp((distance - 0.0) / (gradientWidth - 0.0), 0.0, 1.0);
    float alpha = 1.0 - t * t * (3.0 - 2.0 * t); 
    
    gl_FragColor = vec4(lightColor, alpha);
}