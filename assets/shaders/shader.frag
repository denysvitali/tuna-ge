#version 440 core

layout(binding = 0) uniform sampler2D texSampler;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_emissive;
uniform vec4 material_specular;
uniform int material_shininess;

uniform vec4 light_position; // in eye coordinates
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

in vec2 texCoord;

in vec4 frag_position;
in vec3 normal;

out vec4 frag_out;

void main(void) {
	// Ambient term:
	vec4 frag_color = material_ambient * light_ambient;
	// Diffuse term:
	vec3 _normal = normalize(normal);
	vec3 light_direction = normalize(light_position.xyz - frag_position.xyz);
	float nDotL = dot(light_direction, _normal);
	if (nDotL > 0.0f) {
		frag_color += material_diffuse * nDotL * light_diffuse;
		// Specular term:
		vec3 halfVector = normalize(light_direction.xyz + normalize(-frag_position.xyz));
		float nDotHV = dot(_normal, halfVector);
		frag_color += material_specular * pow(nDotHV, material_shininess) * light_specular;
	}

	vec4 texel = texture(texSampler, texCoord);

	frag_out = vec4(frag_color.xyz, material_ambient.a) * texel;
}