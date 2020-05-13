#version 440 core


in vec3 vsPosition;
in vec3 vsColor;
in vec2 vsTexcoord;
in vec3 vsNormal;

out vec4 fragColor;

uniform sampler2D wallTexture;

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

	//Some problem with vsColor
	//fragColor = vec4(vec3(1.0f, 1.0f, 0.0f), 1.0f);

	//Ambient light
//	vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);
//
//	//Diffuse light
//	vec3 posToLightDirection = normalize(vsPosition - lightPosition);
//	vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);
//	float diffuse = clamp(dot(vsNormal, posToLightDirection), 0, 1);		//Vreau ca diffuse sa fie intre 0-1
//	vec3 diffuseFinal = diffuse * diffuseColor;		// Cat de multa lumina va cadea pe portiunea din obiect

	//Specular light

//
//	fragColor = texture(wallTexture, vsTexcoord) * vec4(vsColor, 1.0f) * 
//				(vec4(ambientLight, 1.0f) + vec4(diffuseFinal, 1.0f));

	vec3 ambient = vec3(0.2);
	vec3 diffuse = vec3(0.5, 0.5, 0.5);
	vec3 specular = vec3(0.95);
	float specPower = 32;
	
	vec3 colorFromTexture = texture2D(wallTexture, vsTexcoord).rgb;

	vec3 color = lighting(vsPosition, vsNormal, lightPosition, viewPosition, ambient, diffuse, specular, specPower) * colorFromTexture;
				
	fragColor = vec4(color, 1.0);
}
