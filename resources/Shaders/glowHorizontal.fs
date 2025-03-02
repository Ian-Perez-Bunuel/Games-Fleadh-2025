#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float resolution;
uniform float radius;

void main()
{
    vec4 sum = vec4(0.0);
    float weight[15] = float[](
        0.196482, 0.186211, 0.166007, 0.139559, 0.109818, 
        0.079647, 0.05195, 0.029042, 0.012959, 0.004877,
        0.001382, 0.000267, 0.000031, 0.000002, 0.0000001
    );

    for (int i = -7; i <= 7; i++)
    {
        float offset = float(i) / resolution * radius;
        sum += texture(texture0, fragTexCoord + vec2(offset, 0.0)) * weight[abs(i)];
    }

    finalColor = sum ;
}
