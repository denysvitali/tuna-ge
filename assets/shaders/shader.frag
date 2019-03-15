#version 440 core

//uniform sampler2D tex;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_emissive;
uniform vec3 material_specular;

//uniform vec3 light_position; // in eye coordinates
//uniform vec3 light_ambient;
//uniform vec3 light_diffuse;
//uniform vec3 light_specular;

//in vec2 tex_coord;

out vec4 fragOutput;

void main(void)
{
//	fragOutput = texture(tex, texCoord);
	fragOutput = vec4(material_emissive, 1.0f);
}