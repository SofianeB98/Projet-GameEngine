#version 130
varying vec2 TexCoord;
varying vec3 Position;

void main()
{
    gl_FragColor = vec4(Position.x, Position.y, sin(Position.x * 0.01 - Position.y * 0.01), 1.0);
}