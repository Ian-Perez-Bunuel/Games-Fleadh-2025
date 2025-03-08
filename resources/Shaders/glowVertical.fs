#ifdef GL_ES
precision mediump float;
#endif
// #version 100  // optional; many compilers ignore or don’t require it

// Replace 'in' with 'varying' for GLSL ES 2.0
varying vec2 fragTexCoord;

uniform sampler2D texture0;
uniform float resolution;
uniform float radius;

void main()
{
    vec4 sum = vec4(0.0);

    // Manually declare and assign the weights (no array constructors in ES 2.0)
    float weight[15];
    weight[0]  = 0.196482;
    weight[1]  = 0.186211;
    weight[2]  = 0.166007;
    weight[3]  = 0.139559;
    weight[4]  = 0.109818;
    weight[5]  = 0.079647;
    weight[6]  = 0.05195;
    weight[7]  = 0.029042;
    weight[8]  = 0.012959;
    weight[9]  = 0.004877;
    weight[10] = 0.001382;
    weight[11] = 0.000267;
    weight[12] = 0.000031;
    weight[13] = 0.000002;
    weight[14] = 0.0000001;

    // Integer loop from -7 to 7
    for (int i = -7; i <= 7; i++)
    {
        float offset = float(i) / resolution * radius;
        
        // Manual absolute value for indexing
        int idx = (i < 0) ? -i : i;

        // Use texture2D in GLSL ES 2.0
        sum += texture2D(texture0, fragTexCoord + vec2(0.0, offset)) * weight[idx];
    }

    // Use gl_FragColor instead of 'out vec4 finalColor'
    gl_FragColor = sum;
}
