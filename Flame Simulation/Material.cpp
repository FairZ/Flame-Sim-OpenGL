#include "Material.h"
#include <SDL.h>
#include <SDL_image.h>
#include <gtc/matrix_transform.hpp>
#include <iostream>

Material::Material(std::string _colourTexName, std::string _normalTexName, Shader* _shader, glm::vec3* _cameraRotation, glm::vec4* _lightPosition)
{
	//show what is loading in console
	std::cout << "Loading: " << _colourTexName.c_str() << std::endl;
	//load in the colour texture
	SDL_Surface* tempTexture = IMG_Load(_colourTexName.c_str()); 
	//if the texture failed to load give error in console
	if(tempTexture==NULL)
	{
		std::cout << "couldn't load: " << _colourTexName << std::endl;
	}
	//set colour format according to the loaded texture
	int colourFormat = GL_RGB;
	if (tempTexture->format->BytesPerPixel == 4)
	{
		colourFormat = GL_RGBA;
	}
	//activate the correct texture unit
	glActiveTexture(GL_TEXTURE0);
	//generate an index
	glGenTextures(1, &m_colourTexture);
	//bind the texture
	glBindTexture(GL_TEXTURE_2D,m_colourTexture);
	//pass in the data
	glTexImage2D(GL_TEXTURE_2D, 0, colourFormat, tempTexture->w, tempTexture->h, 0, colourFormat, GL_UNSIGNED_BYTE, tempTexture->pixels);
	//generate mipmaps and set texture parameters
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//set normal texture index to the colour texture index to avoid texture errors when no normal is loaded
	m_normalTexture = m_colourTexture;
	//delete the surface
	SDL_FreeSurface(tempTexture); 
	
	//if a normal should be loaded
	if (_normalTexName != "NO")
	{
		//show what is loading in console
		std::cout << "Loading: " << _normalTexName.c_str() << std::endl;
		//load in the colour texture
		tempTexture = IMG_Load(_normalTexName.c_str()); 
		//if the texture failed to load give error in console
		if(tempTexture==NULL)
		{
			std::cout << "couldn't load: " << _normalTexName << std::endl;
		}
		//set colour format according to the loaded texture
		colourFormat = GL_RGB;
		if (tempTexture->format->BytesPerPixel == 4)
		{
			colourFormat = GL_RGBA;
		}
		//activate the correct texture unit
		glActiveTexture(GL_TEXTURE1);
		//generate an index
		glGenTextures(1, &m_normalTexture);
		//bind the texture
		glBindTexture(GL_TEXTURE_2D,m_normalTexture);
		//pass in the data
		glTexImage2D(GL_TEXTURE_2D, 0, colourFormat, tempTexture->w, tempTexture->h, 0, colourFormat, GL_UNSIGNED_BYTE, tempTexture->pixels);
		//generate mipmaps and set texture parameters
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//delete the surface
		SDL_FreeSurface(tempTexture); 
	}
	//assign attributes based on input parameters
	m_shader = _shader;
	m_cameraRotation = _cameraRotation;
	m_lightPosition = _lightPosition;
	//set up the projection matrix
	m_projMatrix = glm::perspective(45.0f, 1.7f, 0.1f, 100.0f);
}


void Material::ReadyForDraw(glm::mat4 _modelMatrix)
{
	//set up the view matrix
	m_viewMatrix = glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0,-0.5f,-7.0f)), 0.2f, glm::vec3(1,0,0)),  m_cameraRotation->y + 3.1415f, glm::vec3(0,1,0));
	//use the correct shader
	glUseProgram(m_shader->GetProgram());
	//set the uniforms
	m_shader->SetUniforms(_modelMatrix,m_viewMatrix,m_projMatrix, *m_lightPosition);
	//activate the texture units and bind the textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colourTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture);
}
