#version 440 core

//uniform sampler2D tex;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_emissive;
uniform vec3 material_specular;
uniform int material_shininess;

uniform vec3 light_position; // in eye coordinates
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;

//in vec2 tex_coord;

in vec4 frag_position;
in vec3 normal;

out vec4 frag_out;

void main(void)
{
	// Ambient term:
	vec3 frag_color = material_ambient * light_ambient;
	// Diffuse term:
	vec3 _normal = normalize(normal);
	vec3 light_direction = normalize(light_position - frag_position.xyz);
	float nDotL = dot(light_direction, _normal);
	if (nDotL > 0.0f) {
		frag_color += material_diffuse * nDotL * light_diffuse;
		// Specular term:
		vec3 halfVector = normalize(light_direction + normalize(-frag_position.xyz));
		float nDotHV = dot(_normal, halfVector);
		frag_color += material_specular * pow(nDotHV, material_shininess) * light_specular;
	}
	frag_out = vec4(frag_color, 1.0f);
}