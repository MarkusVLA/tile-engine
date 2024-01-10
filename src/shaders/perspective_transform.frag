#version 120 

varying vec2 TexCoords; 
uniform sampler2D textureSampler;
uniform vec2 resolution;

void main() {
    vec2 p = gl_FragCoord.xy / resolution.xy;
    float depth = 1.0 - p.y;
    float centeringFactor = 0.25;
    p.x = mix(p.x, p.x * (1.0 - centeringFactor) + centeringFactor * 0.5, depth);
    p.y = mix(p.y, p.y * (1.0 - centeringFactor) + centeringFactor * 0.5, depth);
    gl_FragColor = texture2D(textureSampler, p);
}
