#version 430 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec4 vertexWorldPosition_vertex;
out vec3 normal_vertex;
out vec2 texCoord_vertex;
out vec3 tangent_vertex;

void main()
{
	gl_Position = projMat * viewMat * modelMat * position;
	vertexWorldPosition_vertex = modelMat*position;
	normal_vertex =  vec3(modelMat * vec4(normal,0.0));
	texCoord_vertex = texCoord;
	tangent_vertex = vec3(modelMat * vec4(tangent,0.0));
}