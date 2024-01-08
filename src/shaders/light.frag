// light.frag



#version 400 core

in vec2 TexCoords;

uniform vec3 LightPos; // Light position
uniform float LightIntensity; // Light intensity

void main()
{
    // Calculate the distance from the light
    float distance = length(LightPos.xy - TexCoords);

    // Calculate the light intensity at this point
    float intensity = LightIntensity / (distance * distance);

    // Set the color of the pixel
    gl_FragColor = vec4(vec3(intensity), 1.0);
    
}