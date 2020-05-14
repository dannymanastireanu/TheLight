#version 440 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vsPosition;
in vec3 vsColor;
in vec2 vsTexcoord;
in vec3 vsNormal;

out vec4 fragColor;

uniform Material material;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

vec3 lighting(vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
				vec3 ambient, vec3 lightColor, vec3 specular, float specPower)
{
	vec3 L = normalize(lightPos - pos);
	vec3 V = normalize(viewPos - pos);
	vec3 N = normalize(normal);
	vec3 R = normalize(reflect(-L, N));
	float cosTheta = dot(R, V);

	float diffCoef = abs(dot(L, N));
	float specCoef = pow(clamp(cosTheta, 0.0, 1.0), specPower);

	vec3 ambientColor = ambient * lightColor;
	vec3 diffuseColor = diffCoef * lightColor;
	vec3 specularColor = specCoef * specular * lightColor;
	vec3 col = ( ambientColor + diffuseColor + specularColor ); 

	return clamp(col, 0, 1);
}

void main() {

	vec3 ambient = material.ambient;
	vec3 diffuse = material.diffuse;
	vec3 specular = material.specular;
	float specPower = 32;
	
	vec3 colorFromTexture = texture2D(material.diffuseTex, vsTexcoord).rgb;

	vec3 color = lighting(vsPosition, vsNormal, lightPosition, viewPosition, ambient, diffuse, specular, specPower) * colorFromTexture;
				
	fragColor = vec4(color, 1.0);
}
