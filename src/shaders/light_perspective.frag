#version 120 

varying vec2 TexCoords; 
uniform sampler2D textureSampler;
uniform vec2 resolution;

void main() {

    vec2 p = gl_FragCoord.xy / resolution.xy;
    
    // float depth = 1.0 - p.y;
    // float centeringFactor = 0.25;
    // p.x = mix(p.x, p.x * (1.0 - centeringFactor) + centeringFactor * 0.5, depth);
    // p.y = mix(p.y, p.y * (1.0 - centeringFactor) + centeringFactor * 0.5, depth);


    // smoothing
    vec4 color = vec4(0.0);
    float blurSize = 3.0 / resolution.x; 

    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            vec2 offset = vec2(float(x) * blurSize, float(y) * blurSize);
            color += texture2D(textureSampler, p + offset);
        }
    }

    color /= 25.0; 
    gl_FragColor = color;
}
