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
in mat3 vsTBN;

in mat4 vsModelMatrix;

out vec4 FragColor;

uniform Material material;

uniform sampler2D normalMap; 
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
	
	vec3 normalFromMap = texture(material.specularTex, vsTexcoord).rgb;
	normalFromMap.g = 1 - normalFromMap.g;
	normalFromMap = normalFromMap * 2 - 1;
	// With this TBN matrix we can now update the normal mapping code to include the tangent-to-world space transformation:
	normalFromMap = normalize(vsTBN * normalFromMap);

	// Pentru a modifica normala in functie de transformarile geometrice pe care le aplic
	normalFromMap = mat3(transpose(inverse(vsModelMatrix))) * normalFromMap;
	

	//	Without normal map
	//	vec3 color = lighting(vsPosition, vsNormal, lightPosition, viewPosition, ambient, diffuse, specular, specPower) * colorFromTexture;
	
	//	With normal map
	vec3 color = lighting(vsPosition, normalFromMap, lightPosition, viewPosition, ambient, diffuse, specular, specPower) * colorFromTexture;
	
	FragColor = vec4(color, 1.0);
}
