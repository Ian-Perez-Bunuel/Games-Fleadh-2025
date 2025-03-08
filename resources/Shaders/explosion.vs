#ifdef GL_ES
precision mediump float;
#endif

// Input vertex attributes
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform float displacementIntensity;
uniform sampler2D perlinNoiseTexture;

// Output vertex attributes
varying vec4 fragColor;
varying vec2 fragTexCoord;
varying vec3 fragNormal;

void main()
{
    vec4 noise = texture2D(perlinNoiseTexture, vertexTexCoord);
    
    vec3 displacedPosition = vertexPosition + (vertexNormal * noise.r * displacementIntensity);
    
    fragColor = vertexColor;
    
    gl_Position = mvp * vec4(displacedPosition, 1.0);
    
    fragTexCoord = vertexTexCoord;
    fragNormal = vertexNormal;
}
