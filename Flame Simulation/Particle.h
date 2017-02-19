#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include <glm.hpp>
#include <vector>

//particle class containing neccesary per-particle information and functions for calculating movement and distance of one particle to another
//as well as some simple get functions
class Particle
{
private:
	glm::vec3 m_position;
	int m_lifetime;
	glm::vec3 m_velocity;
public:
	Particle();
	void Update(std::vector<Particle*> &_particlesInRange, unsigned int _deltaTime); 
	glm::vec3 GetPos(){return m_position;}
	int GetLifetime(){return m_lifetime;}
	float Magnitude(glm::vec3 _vector){return (_vector.x*_vector.x)+(_vector.y*_vector.y)+(_vector.z*_vector.z);}
};

#endif