#version 440 core

uniform mat4 projection;
uniform mat4 modelview;
uniform mat3 normal_matrix;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Texture;
layout(location = 2) in vec3 in_Normal;

out vec2 texCoord;
out vec4 frag_position;
out vec3 normal;

void main(void)
{
    frag_position = modelview * vec4(in_Position, 1.0f);
    gl_Position = projection * frag_position;
    normal = normal_matrix * in_Normal;
    texCoord = in_Texture;
}