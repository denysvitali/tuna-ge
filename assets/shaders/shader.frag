#version 440 core

in vec2 texCoord;

uniform vec4 color;
out vec4 fragOutput;
// Texture mapping:
uniform sampler2D texSampler;
void main(void)
{
    // Texture element:
    vec4 texel = texture(texSampler, texCoord);

    // Final color:
    fragOutput = texel;
}
