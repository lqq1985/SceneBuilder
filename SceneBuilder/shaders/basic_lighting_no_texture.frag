#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
  
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 lightColor = vec3(1,1,1);
vec3 objectColor = vec3(0.6, 0.6, 0.6);
uniform float shininess = 32.0f;
uniform vec3 material_specular = vec3(0.1f, 0.1f, 0.1f);
uniform vec3 light_specular = vec3(0.5f, 0.5f, 0.5f);

void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light_specular * (spec * material_specular);  
            
    vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
