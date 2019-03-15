#version 440 core

uniform mat4 projection;
uniform mat4 modelview;

layout(location = 0) in vec3 in_Position;

out vec3 out_Color;
out float dist;

void main(void)
{
    gl_Position = projection * modelview * vec4(in_Position, 1.0f);
    dist = abs(gl_Position.z / 100.0f);
    out_Color = vec3(255, 0, 0);
}