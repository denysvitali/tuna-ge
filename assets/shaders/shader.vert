#version 440 core
// Uniforms:
uniform mat4 projection;
uniform mat4 modelview;
uniform mat3 normalMatrix;
// Attributes:
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Texture;
// Varying:
out vec4 fragPosition;
out vec3 normal;
out vec2 texCoord;
void main(void)
{
    fragPosition = modelview * vec4(in_Position, 1.0);
    gl_Position = projection * fragPosition;
    normal = normalMatrix * in_Normal;
    texCoord = in_Texture;
}