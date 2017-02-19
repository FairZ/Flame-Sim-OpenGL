#ifndef _FLAME_H_
#define _FLAME_H_
#include "Particle.h"
#include "glew.h"
#include <vector>
#include "Material.h"

//definition of the maximum number of particles
#define NUMBEROFPARTICLES 1500

//KDTree class used to spatially partition the particles for faster interactions
//each instance is a node of the tree, containing references to its left and right nodes and a particle
//the class contains functions used in the generation, deletion and searching of the tree
class KDTree
{
private:
	int m_depth;
	Particle* m_point;
	KDTree* m_left;
	KDTree* m_right;
public:
	KDTree(int _depth, std::vector<Particle*> _particles);
	void DestroyTree();
	void PrintTree(); //for debug only
	int FindMedian(float _start, float _middle, float _end);
	void Swap(std::vector<Particle*> _particles, int _index1, int _index2);
	//magnitude function simply qorks out the squared magnitude of a vector
	float Magnitude(glm::vec3 _vector){return (_vector.x*_vector.x)+(_vector.y*_vector.y)+(_vector.z*_vector.z);}
	void FindParticlesInRange(float _range, glm::vec3 _position, std::vector<Particle*> &_particlesInRange, float* _largestDistanceInRange);
};


//the flame class acts as a container and intermediary of the particles and KDTree
//it also contains its own VAO which it buffers particle positions to each frame and some other neccesary data such as a material
//it contains functions for updating all the particles and drawing the flame
class Flame
{
private:
	KDTree* m_root;
	std::vector<Particle*> m_particles;
	std::vector<Particle*> m_particlesInRange;
	GLuint m_VAO;
	GLuint m_posBuffer;
	float m_positions[NUMBEROFPARTICLES*3];
	unsigned int m_numParticles;
	Material* m_material;
	unsigned int* m_deltaTime;
public:
	Flame(Material* _material, unsigned int* _deltaTime);
	~Flame();
	void Update();
	void Draw();
};

#endif