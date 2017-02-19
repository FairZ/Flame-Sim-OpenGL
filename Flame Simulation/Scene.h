#ifndef _SCENE_H_
#define _SCENE_H_
#include <SDL.h>
#include <vector>
#include <memory>
#include <glm.hpp>
#include "Object.h"
#include "Material.h"
#include "Mesh.h"
#include "Flame.h"


//contains references to all objects, flames, shaders, materials and meshes in the scene
//as well as having control over the camera and light position
//has functions which cause the whoel scene to be updated and drawn from a single call
class Scene
{
private:
	std::vector<Object*> m_objects;
	std::vector<Flame*> m_fires;
	Shader *m_logShader;
	Shader *m_pointsShader;
	Shader *m_grassShader;
	Material *m_logMat;
	Material *m_grassMat;
	Material *m_particleMat;
	Mesh *m_logMesh;
	Mesh *m_grassMesh;
	glm::vec3 m_cameraRotation;
	glm::vec4 m_lightPosition;
	Uint32 m_deltaTime;
	Uint32 m_previousTime;
	bool m_fireUp;
	bool m_rotateRight;
	bool m_rotateLeft;
public:
	Scene(void);
	void Draw();
	void Update();
	void SetDeltaTime(unsigned int _deltaTime){m_deltaTime = _deltaTime;}
	void SetRotateRight(bool _rotate){m_rotateRight = _rotate;}
	void SetRotateleft(bool _rotate){m_rotateLeft = _rotate;}
};

#endif