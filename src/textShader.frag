#version 120

uniform sampler2D tex;
varying vec2 texcoords;
//out vec4 outColor;

void main()

{
    gl_FragColor = vec4(0.0, 0.0, 0.0, texture2D(tex, texcoords).r);
    //outColor = vec4(0.18, 0.18, 0.18, 1.0);
}
