#version 440 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D textureImg;
	sampler2D normalTextureImg;
};

in vec3 vsPosition;
in vec3 vsColor;
in vec2 vsTexcoord;
in vec3 vsNormal;
in mat3 vsTBN;
in vec4 FragPosLightSpace;

in mat4 vsModelMatrix;

out vec4 FragColor;

uniform Material material;

uniform sampler2D normalMap; 
uniform vec3 lightPosition;
uniform vec3 viewPosition;

// Shadow-mapping
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

	// Until I will resolve the bug to do not affect entire project: return 0
	return 0.0;
    return shadow;
}

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

	float shadow = ShadowCalculation(FragPosLightSpace);

	vec3 col = ( ambientColor + (1.0 - shadow) * ( diffuseColor + specularColor )); 
	
	return clamp(col, 0, 1);
}

void main() {

	vec3 ambient = material.ambient;
	vec3 diffuse = material.diffuse;
	vec3 specular = material.specular;
	float specPower = 32;
	
	vec3 colorFromTexture = texture2D(material.textureImg, vsTexcoord).rgb;
	
	vec3 normalFromMap = texture(material.normalTextureImg, vsTexcoord).rgb;
	normalFromMap.g = 1 - normalFromMap.g;
	normalFromMap = normalize(normalFromMap * 2 - 1);

	// With this TBN matrix we can now update the normal mapping code to include the tangent-to-world space transformation:
	normalFromMap = normalize(vsTBN * normalFromMap);

	

	//	Without normal map
//	vec3 color = lighting(vsPosition, vsNormal, lightPosition, viewPosition, ambient, diffuse, specular, specPower) * colorFromTexture;
	
	//	With normal map
	vec3 color = lighting(vsPosition, normalFromMap, lightPosition, viewPosition, ambient, diffuse, specular, specPower) * colorFromTexture;
	
	FragColor = vec4(color, 1.0);
}
