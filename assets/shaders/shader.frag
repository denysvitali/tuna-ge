#version 440 core

in vec3 out_Color;
in float dist;
   
out vec4 fragOutput;

void main(void)
{
    vec3 fog = vec3(1.0f, 1.0f, 1.0f);
    fragOutput = vec4(mix(out_Color, fog, dist), 1.0f);
}