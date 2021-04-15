#version 120

attribute vec3 a_Position;
attribute vec2 a_TexCoords;
attribute vec3 a_normals;

varying vec2 TexCoord;
varying vec3 Position;
varying vec3 v_normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    v_normals = mat3(model) * a_normals; //mat3(transpose(inverse(model)))
    Position = vec3(model * vec4(a_Position, 1.0f)); //
    TexCoord = a_TexCoords;
    gl_Position = projection * view * model * vec4(a_Position, 1.0f);
}