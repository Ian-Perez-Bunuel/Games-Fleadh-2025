#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float resolution;
uniform float radius;

void main()
{
    vec4 sum = vec4(0.0);
    float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    for (int i = -4; i <= 4; i++)
    {
        float offset = float(i) / resolution * radius;
        sum += texture(texture0, fragTexCoord + vec2(offset, 0.0)) * weight[abs(i)];
    }

    finalColor = sum;
}
