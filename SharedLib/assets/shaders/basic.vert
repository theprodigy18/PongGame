#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

layout (location = 0) out vec3 outColor;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	mat4 mv = projection * model;
	gl_Position = mv * vec4(position, 1.0f);
	outColor = color;
}