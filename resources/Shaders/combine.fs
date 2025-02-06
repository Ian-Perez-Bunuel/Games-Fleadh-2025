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

    // Screen Blending: Avoids excessive stacking of opacity
    finalColor = 1.0 - ((1.0 - sceneColor) * (1.0 - glowColor));

    // Optional: If too bright, mix with the original
    finalColor = mix(sceneColor, finalColor, 0.6);
}
