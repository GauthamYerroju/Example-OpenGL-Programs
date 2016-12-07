#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;
layout (location = 2) in vec3 v_normal;

out vec3 f_position;
out vec3 f_normal;
out vec2 texCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix; 


void main(void)
{
	// Transform vertex position into view coordinates
	f_position = ( (viewMatrix *modelMatrix) * vec4(v_position, 1.0f)).xyz;
	f_normal = ( (viewMatrix * modelMatrix) * vec4(v_normal, 0.0)).xyz;
	texCoord = v_texCoord;

    gl_Position = (projectionMatrix * viewMatrix *  modelMatrix) * vec4(v_position, 1.0f);
}