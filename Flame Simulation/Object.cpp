#include "Object.h"
#include <gtc/matrix_transform.hpp>

Object::Object(Mesh* _mesh, Material* _material, unsigned int* _deltaTime, glm::vec3 _scale, glm::vec3 _position, glm::vec3 _rotation)
{
	//assign attributes based on input parameters
	m_mesh = _mesh;
	m_material = _material;
	m_deltaTime = _deltaTime;
	m_modelMatrix = glm::mat4();
	m_rotation = _rotation;
	m_position = _position;
	m_scale = _scale;
}

void Object::Draw()
{
	//ready the material and draw the mesh
	m_material->ReadyForDraw(m_modelMatrix);
	m_mesh->Draw();
}

void Object::Update()
{
	//update the model matrix
	m_modelMatrix = glm::translate(glm::mat4(1.0f),m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.y,glm::vec3(0,1.0f,0));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.x,glm::vec3(1.0f,0,0));
	m_modelMatrix = glm::scale(m_modelMatrix,m_scale);
}