#ifdef GL_ES
precision mediump float;
#endif

// Replace 'in' with 'varying'
varying vec2 fragTexCoord;
varying vec4 fragColor;    // Passed from the vertex shader
varying vec3 fragNormal;

uniform vec4 color;    // Comes from the code

void main()
{    
    gl_FragColor = fragColor * color;
}
