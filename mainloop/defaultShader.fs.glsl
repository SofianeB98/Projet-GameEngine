#version 130
varying vec2 TexCoord;

void main()
{
    gl_FragColor = vec4(1.0, 1.0, TexCoord.x * TexCoord.y, 1.0);
}