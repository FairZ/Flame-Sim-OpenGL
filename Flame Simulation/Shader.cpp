#include "Shader.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>

//constructor creates shaders with different configurations based on the input parameter
Shader::Shader(unsigned int _switch)
{
	if (_switch == 1) //a shader with a texture and normal map
	{
		//load in the appropriate shaders from file
		std::string vertexString = LoadShader("ModelVertexNormal.txt");
		m_vertex = vertexString.c_str();
		std::string fragmentString = LoadShader("ModelFragmentNormal.txt");
		m_fragment = fragmentString.c_str();

		//generate a program
		m_program = glCreateProgram();

		//create indexes and push shader data to the graphics card whilst attaching them to the program
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex,1,&m_vertex,NULL);
		glCompileShader(vertex);
		glAttachShader(m_program, vertex);

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment,1,&m_fragment,NULL);
		glCompileShader(fragment);
		glAttachShader(m_program, fragment);

		//link the program together
		glLinkProgram(m_program);

		//find appropriate uniform indexes
		m_modelMatLocation = glGetUniformLocation(m_program, "modelMat");
		m_viewMatLocation = glGetUniformLocation( m_program, "viewMat" );
		m_projectionMatLocation = glGetUniformLocation(m_program, "projMat");
		m_colourTextureSampleLocation = glGetUniformLocation(m_program, "colourTexture");
		m_normalTextureSampleLocation = glGetUniformLocation(m_program, "normalTexture");
		m_lightPositionLocation = glGetUniformLocation(m_program, "worldSpaceLightPosition");
	}
	else if (_switch == 0) //a particle shader
	{
		//load in the appropriate shaders from file, note this includes a geometry shader
		std::string vertexString = LoadShader("ParticleVertex.txt");
		m_vertex = vertexString.c_str();

		std::string geometryString = LoadShader("ParticleGeometry.txt");
		m_geometry = geometryString.c_str();

		std::string fragmentString = LoadShader("ParticleFragment.txt");
		m_fragment = fragmentString.c_str();

		//generate a program
		m_program = glCreateProgram();

		//create indexes and push shader data to the graphics card whilst attaching them to the program
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex,1,&m_vertex,NULL);
		glCompileShader(vertex);
		glAttachShader(m_program, vertex);

		GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry,1,&m_geometry,NULL);
		glCompileShader(geometry);
		glAttachShader(m_program, geometry);

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment,1,&m_fragment,NULL);
		glCompileShader(fragment);
		glAttachShader(m_program, fragment);

		//link the program together
		glLinkProgram(m_program);

		//find appropriate uniform indexes
		m_modelMatLocation = glGetUniformLocation(m_program, "modelMat");
		m_viewMatLocation = glGetUniformLocation(m_program, "viewMat" );
		m_projectionMatLocation = glGetUniformLocation(m_program, "projMat");
		m_colourTextureSampleLocation = glGetUniformLocation(m_program, "colourTexture");
	}
	else if (_switch = 2) //a shader with just a texture (note: not currently used, simply here for later expansion)
	{
		//load in the appropriate shaders from file
		std::string vertexString = LoadShader("ModelVertex.txt");
		m_vertex = vertexString.c_str();
		std::string fragmentString = LoadShader("ModelFragment.txt");
		m_fragment = fragmentString.c_str();

		//generate a program
		m_program = glCreateProgram();

		//create indexes and push shader data to the graphics card whilst attaching them to the program
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex,1,&m_vertex,NULL);
		glCompileShader(vertex);
		glAttachShader(m_program, vertex);

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment,1,&m_fragment,NULL);
		glCompileShader(fragment);
		glAttachShader(m_program, fragment);

		//link the program together
		glLinkProgram(m_program);

		//find appropriate uniform indexes
		m_modelMatLocation = glGetUniformLocation(m_program, "modelMat");
		m_viewMatLocation = glGetUniformLocation( m_program, "viewMat" );
		m_projectionMatLocation = glGetUniformLocation(m_program, "projMat");
		m_colourTextureSampleLocation = glGetUniformLocation(m_program, "colourTexture");
		m_lightPositionLocation = glGetUniformLocation(m_program, "worldSpaceLightPosition");
	}
}

//function to load shaders from file
std::string Shader::LoadShader(const char* _fileName)
{
	std::string retVal;
	std::string line;
	//open a filestream to the specified location
	std::ifstream fileStream(_fileName, std::ios::in);

	//error checking
	if(!fileStream.is_open())
	{
		std::cout << "could not open: " << _fileName << std::endl;
		return "";
	}

	//while there is still data to read, add a line to the return string with appropriate line ending
	while(!fileStream.eof())
	{
		std::getline(fileStream,line);
		retVal.append(line + "\n\ ");
	}

	//close the filestream
	fileStream.close();
	//return the final string
	return retVal;
}

//function to set the uniforms before drawing
void Shader::SetUniforms(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix, glm::vec4 _lightPos)
{
	glUniformMatrix4fv(m_modelMatLocation,1,GL_FALSE, glm::value_ptr(_modelMatrix));
	glUniformMatrix4fv(m_viewMatLocation,1,GL_FALSE, glm::value_ptr(_viewMatrix));
	glUniformMatrix4fv(m_projectionMatLocation,1,GL_FALSE, glm::value_ptr(_projectionMatrix));

	//if the shader has no light position (in the case of the particle function) do not assign the uniform
	if (m_lightPositionLocation != NULL)
	{
		glUniform4fv(m_lightPositionLocation,1,glm::value_ptr(_lightPos));
	}
	glUniform1i(m_colourTextureSampleLocation, 0);
	glUniform1i(m_normalTextureSampleLocation, 1);
}