#version 330

uniform sampler2D tex;
in vec2 texcoords;
out vec4 outColor;

void main()

{
    outColor = vec4(0.18, 0.18, 0.18, texture2D(tex, texcoords).r);
    //outColor = vec4(0.18, 0.18, 0.18, 1.0);
}
