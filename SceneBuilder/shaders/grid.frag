#version 330 core

in vec3 vPos;

uniform vec3 colour;
uniform vec3 viewPos;

float alpha = 1.0f;

void main()
{
    // use discard to remove a pixel
    if (vPos.y == 0 && vPos.x == 0) {
        gl_FragColor = vec4(vec3(1,0,0), 1.0f);
    } else if (vPos.x == 0) {
        gl_FragColor = vec4(vec3(0,1,0), 1.0f);
    } else if (vPos.z == 0) {
        gl_FragColor = vec4(vec3(0,0,1), 1.0f);
    } else {
        gl_FragColor = vec4(colour, 1.0f); 
    }
}