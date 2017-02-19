#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "Material.h"
#include "Mesh.h"
#include <memory>

//object class containing references to a material and a mesh as well as model scale, rotation and position in the form of a model matrix
//also contains functions to update and draw the object
class Object
{
private:
	Material* m_material;
	Mesh* m_mesh;
	unsigned int* m_deltaTime;
	glm::mat4 m_modelMatrix;
	glm::vec3 m_scale;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
public:

	Object(Mesh* _mesh, Material* _material, unsigned int* _deltaTime, glm::vec3 _scale, glm::vec3 _position, glm::vec3 _rotation);
	void Draw();
	void Update();
};

#endif