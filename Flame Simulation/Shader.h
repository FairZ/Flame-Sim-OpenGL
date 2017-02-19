#ifndef _SHADER_H_
#define _SHADER_H_
#include "glew.h"
#include <glm.hpp>
#include <string>

//shader class containing indexes of uniform locations and the shader program itself
//also contains functions for loading shaders from file and setting uniforms before drawing
//materials have a pointer to a shader
class Shader
{
private:
	bool m_compiled;
	GLint m_program;
	GLint m_modelMatLocation;
	GLint m_viewMatLocation;
	GLint m_projectionMatLocation;
	GLint m_lightPositionLocation;
	GLint m_colourTextureSampleLocation;
	GLint m_normalTextureSampleLocation;
	const GLchar* m_vertex;
	const GLchar* m_fragment;
	const GLchar* m_geometry;
public:
	Shader(unsigned int _switch);
	void SetUniforms(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix, glm::vec4 _lightPos);
	GLint GetProgram(){return m_program;}
	std::string LoadShader(const char* _fileName);
};

#endif