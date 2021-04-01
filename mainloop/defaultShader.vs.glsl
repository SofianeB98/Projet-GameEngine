#version 130

attribute vec3 a_Position;
attribute vec2 a_TexCoords;

varying vec2 TexCoord;
varying vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_Position, 1.0f);
    TexCoord = vec2(a_TexCoords.x, 1.0 - a_TexCoords.y);
    Position = vec3(gl_Position);
}