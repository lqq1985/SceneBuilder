#version 330 core
out vec4 FragColor;

//in
in vec3 fragmentPosition;
in vec3 normal;
in vec2 textureCoord;

// uniform
uniform sampler2D diffuseTexture;
uniform vec3 lightPos;

// properties
float lightAmbienceStrength = 0.4;
vec3 lightDiffuseStrength = vec3(0.1,0.1,0.1);

void main() {
	// ambience
	vec3 ambient = lightAmbienceStrength * texture(diffuseTexture, textureCoord).rgb;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - fragmentPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = lightDiffuseStrength * diff * texture(diffuseTexture, textureCoord).rgb; 

	vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}