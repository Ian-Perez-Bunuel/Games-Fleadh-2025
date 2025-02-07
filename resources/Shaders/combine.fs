#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D originalScene;
uniform sampler2D blurredGlow;
uniform float intensity;

void main()
{
    vec4 sceneColor = texture(originalScene, fragTexCoord);
    vec4 glowColor = texture(blurredGlow, fragTexCoord) * intensity;

    // HDR-style blending for more realistic light spread
    finalColor = 1.0 - exp(-sceneColor - glowColor);

    // Ensure glow is balanced and not too strong
    finalColor = mix(sceneColor, finalColor, 0.7);
}
