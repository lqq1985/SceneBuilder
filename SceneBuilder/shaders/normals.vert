#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexture;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float scale;

void main()
{
	vec3 position = vec3(model * vec4(aPos, 1.0));
	vec3 fragPos = vec3(position.x, position.y*scale, position.z);
    gl_Position = projection * view * vec4(fragPos, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * aNormal, 0.0)));
	vs_out.tangent = normalize(vec3(projection * vec4(normalMatrix * aTangent, 1.0)));
	vs_out.bitangent = normalize(vec3(projection * vec4(normalMatrix * aBitangent, 1.0)));
}

