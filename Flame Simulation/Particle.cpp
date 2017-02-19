#include "Particle.h"
#include <random>
#include <time.h>
#include "Flame.h"


//contructor giving a random starting position (within range)
Particle::Particle()
{
	m_lifetime = 200;
	m_velocity = glm::vec3(0,0,0);
	m_position = glm::vec3((((float) rand() / (float) RAND_MAX) - 0.5)/3,-0.5,(((float) rand() / (float) RAND_MAX) - 0.5)/3);
}

//update loop goes through all of the particles within a small range of itself and either accelerates towards or away from them depending on density
//though not quite a full fluid dynamics equation it gives a good substitute at a low cost to the cpu
void Particle::Update(std::vector<Particle*> &_particlesInRange, unsigned int _deltaTime)
{
	glm::vec3 acceleration;
	//upwards acceleration due to convection added
	acceleration.y += 0.6f;
	//lifetime of the particle decreased
	m_lifetime--;
	//for each particle in range move towards or away from it depending on density 
	for (unsigned int i = 0; i < _particlesInRange.size(); i++)
	{
		if (_particlesInRange[i] != this)
		{
			if (_particlesInRange.size() > 20)
			{
				acceleration += (m_position - _particlesInRange[i]->m_position) * 0.02f;
			}
			else
			{
				acceleration += (m_position - _particlesInRange[i]->m_position) * -2.0f;
			}
		}
	}
	//accelerate the particle
	m_velocity += acceleration * ((float)_deltaTime / 1000.0f);
	//move the particle
	m_position += m_velocity * ((float)_deltaTime / 1000.0f);
}
