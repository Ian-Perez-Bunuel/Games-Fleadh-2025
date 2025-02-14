#version 330

in vec2 fragTexCoord;
out vec4 finalColor;
uniform sampler2D texture0;
uniform vec2 resolution;  // 🔥 Fixes hardcoded 800x800 resolution

float warp = 0.15;  // 🔥 Reduced to be more realistic
float scanIntensity = 0.5; // 🔥 Adjust darkness between scanlines

void main()
{
    vec2 uv = fragTexCoord;

    // 🔹 Apply CRT curvature warp effect
    vec2 offset = (uv - 0.5) * 2.0;  // Centered UV
    offset.x *= 1.0 + (offset.y * offset.y) * warp;
    offset.y *= 1.0 + (offset.x * offset.x) * warp;
    uv = offset * 0.5 + 0.5;  // Scale back to normalized UV range

    // 🔹 If outside screen, set black
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
    {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // 🔹 Sample the texture at the warped coordinates
    vec4 texColor = texture(texture0, uv);

    // 🔹 Apply scanline effect based on pixel row
    float scanline = 0.5 + 0.5 * sin(uv.y * resolution.y * 3.1415); // 🔥 Fixed scanline issue
    texColor.rgb *= mix(1.0, scanline, scanIntensity); // 🔥 Blend texture with scanlines

    finalColor = texColor;
}
