#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform float displacementIntensity;
uniform float explosionTimer;

// Output vertex attributes
out vec4 fragColor;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main()
{
    // Calculate displacement along the vertex normal
    vec3 displacedPosition = vertexPosition + (vertexNormal * displacementIntensity * explosionTimer);
    
    fragColor = vertexColor;
    // Calculate final vertex position
    // MVP Model View Projection
    gl_Position = mvp * vec4(displacedPosition, 1.0);
    
    // Pass vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragNormal = vertexNormal;
}

