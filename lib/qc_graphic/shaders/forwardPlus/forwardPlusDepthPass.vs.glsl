#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uModelViewProjMatrix;

void main()
{
	gl_Position = uModelViewProjMatrix * vec4(aPosition, 1);
}
