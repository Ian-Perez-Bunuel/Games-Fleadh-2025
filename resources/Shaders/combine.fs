#ifdef GL_ES
precision mediump float;
#endif

// WebGL 1 / GLSL ES 2.0 style
// #version 100  // Optional: many ES compilers accept it or you can omit it

varying vec2 fragTexCoord;

uniform sampler2D originalScene;
uniform sampler2D blurredGlow;
uniform float intensity;

void main()
{
    vec4 sceneColor = texture2D(originalScene, fragTexCoord);
    vec4 glowColor = texture2D(blurredGlow, fragTexCoord) * intensity;

    // HDR-style blending for more realistic light spread
    vec4 hdrColor = 1.0 - exp(-sceneColor - glowColor);

    // Ensure glow is balanced and not too strong
    vec4 finalColor = mix(sceneColor, hdrColor, 0.7);

    gl_FragColor = finalColor;
}
