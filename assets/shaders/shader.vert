#version 440 core

uniform mat4 projection;
uniform mat4 modelview;
uniform mat3 normal_matrix;

layout(location = 0) in vec3 in_position;
//layout(location = 1) in vec2 in_tex_coord;
layout(location = 2) in vec3 in_normal;

//out vec2 tex_coord;
out vec4 frag_position;
out vec3 normal;

void main(void)
{
	frag_position = modelview * vec4(in_position, 1.0f);
    gl_Position = projection * frag_position;
	normal = normal_matrix * in_normal;
//    texCoord = in_tex_coord;
}