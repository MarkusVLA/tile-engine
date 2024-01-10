// Fragment Shader
#version 120

varying vec2 TexCoord;

uniform vec2 bulletPos; // Bullet position in SFML coordinates
uniform vec2 bulletDirection; // Direction of the bullet
uniform vec2 cameraPos; // Position of view.
uniform vec2 renderTargetRes;

void main() {
    vec2 uv = gl_FragCoord.xy / renderTargetRes.xy;
    // Convert SFML coordinates to OpenGL
    vec2 screenPos = ((cameraPos + renderTargetRes) - bulletPos) / renderTargetRes;
    vec2 bullet = screenPos - vec2(0.5);
    bullet.x = 1.0 - bullet.x;
    vec2 direction = bulletDirection;
    direction.x = -1.0 * direction.x;

    float lineLength = 0.06; 
    float maxLineThickness = 0.006; 

    vec2 normBulletDir = normalize(direction);

    float t = dot(uv - bullet, normBulletDir);
    t = clamp(t, 0.0, lineLength);
    vec2 closestPoint = bullet + t * normBulletDir;

    float distance = length(uv - closestPoint);

    vec3 bulletColor = vec3(1.0, 1.0, 1.0); // Wite buller reacts to surrounding light

    float thickness = mix(maxLineThickness, 0.0, t / lineLength);
    float alpha = distance < thickness ? mix(1.0, 0.0, t / lineLength) : 0.0;

    gl_FragColor = vec4(bulletColor, alpha);
}
