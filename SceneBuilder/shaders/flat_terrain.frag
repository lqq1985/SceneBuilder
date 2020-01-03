#version 330 core
out vec4 FragColor;

flat in vec4 polygon_color;

void main()
{
	FragColor = polygon_color;
}
