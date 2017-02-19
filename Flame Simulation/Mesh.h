#ifndef _MESH_H_
#define _MESH_H_
#include <string>
#include "glew.h"

//mesh object containing index of mesh VAO and the number of verteces in said mesh
//also contains functions for loading an obj from file and drawing the mesh
class Mesh
{
private:
	GLuint m_VAO;
	unsigned int m_numVertices;
public:
	Mesh(){}
	Mesh(std::string _fileName);
	~Mesh(void);
	void LoadOBJ(std::string _fileName);
	void Draw();
};

#endif