#version 430 core
layout(location = 0) in vec4 vPosition;

uniform mat4 modelMat;
uniform mat4 viewMat;

void main(){
	gl_Position = viewMat * modelMat * vPosition;
}