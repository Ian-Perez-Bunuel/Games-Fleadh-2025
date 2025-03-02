#version 330

// Input vertex attributes from vertex shader
in vec2 fragTexCoord;
in vec4 fragColor;    // Comes from the vertex shader
in vec3 fragNormal;

uniform vec4 color;    // Comes from the code

// Final color output
out vec4 finalColor;

void main()
{
    // Basic lighting calculation
    // Can use new vertex normal for lighting
    // vec3 lightDir = normalize(vec3(1.0, 1.0, -1.0));
    // float intensity = max(dot(fragNormal, lightDir), 0.0);
    
    finalColor = fragColor * color;
}