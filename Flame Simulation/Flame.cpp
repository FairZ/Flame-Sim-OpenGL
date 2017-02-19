#include "Flame.h"
#include "Shader.h"
#include <gtc/matrix_transform.hpp>
#include <iostream>

//expand for KDTree implementation
#pragma region KDTree

//recursive constructor finds median of a set using the median of three algorithm
KDTree::KDTree(int _depth, std::vector<Particle*> _particles)
{
	//intialise median to centre of set
	int median = _particles.size()/2;
	//if there are 3 or more particles in the set find the median using median of three algorithm
	if (_particles.size() >= 3)
	{
		int mid;
		//depending on the depth, split by a different axis
		switch(_depth % 3)
		{
		case 1:
			//median of three algorithm
			mid = FindMedian(_particles.front()->GetPos().x, _particles[median]->GetPos().x, _particles.back()->GetPos().x);
			//switch the middle value to the centre of the set
			switch (mid)
			{
			case 1:
				Swap(_particles,0,median);
				break;
			case 3:
				Swap(_particles,_particles.size()-1,median);
				break;
			}
			break;
		case 2:
			mid = FindMedian(_particles.front()->GetPos().y, _particles[median]->GetPos().y, _particles.back()->GetPos().y);
			switch (mid)
			{
			case 1:
				Swap(_particles,0,median);
				break;
			case 3:
				Swap(_particles,_particles.size()-1,median);
				break;
			}
			break;
		case 0:
			mid = FindMedian(_particles.front()->GetPos().z, _particles[median]->GetPos().z, _particles.back()->GetPos().z);
			switch (mid)
			{
			case 1:
				Swap(_particles,0,median);
				break;
			case 3:
				Swap(_particles,_particles.size()-1,median);
				break;
			}
			break;
		}		
	}
	//assign the point to the median of the set
	m_point = _particles[median];
	m_left = nullptr;
	m_right = nullptr;
	m_depth = _depth;
	std::vector<Particle*> leftSubSet;
	std::vector<Particle*> rightSubSet;

	//create subsets out of the left over particles
	for (int i = 0; i < median; i++)
	{
		leftSubSet.push_back(_particles[i]);
	}

	for (int i = median+1; i < _particles.size(); i++)
	{
		rightSubSet.push_back(_particles[i]);
	}

	//if the subsets are larger than 0 recursively create a new tree node
	if (rightSubSet.size() > 0)
	{
		m_right = new KDTree(_depth+1, rightSubSet);
	}
	if (leftSubSet.size() > 0)
	{
		m_left = new KDTree(_depth+1, leftSubSet);
	}
}


int KDTree::FindMedian(float _start, float _middle, float _end)
{
	//median of three algorithm, return 1, 2 or 3 depending on if the median value is the start, middle or end of the set respectively
	if (_start > _end)
	{
		if (_middle > _start)
		{
			return 1;
		}
		if (_end > _middle)
		{
			return 3;
		}
		return 2;
	}
	else
	{
		if (_start > _middle)
		{
			return 1;
		}
		if (_middle > _end)
		{
			return 3;
		}
		return 2;
	}
}

//simple function to swap the given values within a vector
void KDTree::Swap(std::vector<Particle*> _particles, int _index1, int _index2)
{
	Particle* temp = _particles[_index1];
	_particles[_index1] = _particles[_index2];
	_particles[_index2] = temp;
}

//recursively go down the tree and delete nodes on the way back up
void KDTree::DestroyTree()
{
	if(m_right != nullptr)
	{
		m_right->DestroyTree();
		delete m_right;
	}
	if(m_left != nullptr)
	{
		m_left->DestroyTree();
		delete m_left;
	}
}

//print function for debugging
void KDTree::PrintTree()
{
	//print the tree in order from below the node that called the function
	if (m_left != nullptr)
	{
		m_left->PrintTree();
	}

	std::cout << m_point->GetPos().x << " " << m_point->GetPos().y << " " << m_point->GetPos().z << " : " << m_depth << std::endl;

	if (m_right != nullptr)
	{
		m_right->PrintTree();
	}
}

//recursive function to find all the particles within a range of a particle
void KDTree::FindParticlesInRange(float _squaredRange, glm::vec3 _position, std::vector<Particle*> &_particlesInRange, float* _largestDistanceInRange)
{
	///set the distance between the node being searched and the particle
	float distance = Magnitude(_position - m_point->GetPos());
	//if the distance is smaller than the smallest distance which is greater than the range checked so far, continue with checks
	if (distance < *_largestDistanceInRange)
	{
		//if the distance is within the range, add this node's particle to the vector
		if (distance <= _squaredRange)
		{
			_particlesInRange.push_back(m_point);
		}
		//otherwise lower the smallest distance which is greater than the range
		else
		{
			*_largestDistanceInRange = distance;
		}
		//if there are nodes beneath the one being checked, call the function in those nodes
		if (m_left != nullptr)
		{
			m_left->FindParticlesInRange(_squaredRange,_position, _particlesInRange, _largestDistanceInRange);
		}
		if (m_right != nullptr)
		{
			m_right->FindParticlesInRange(_squaredRange,_position, _particlesInRange, _largestDistanceInRange);
		}
	}
	//else discard this node being checked and it's children
}

#pragma endregion

//expand for Flame implementation
#pragma region Flame

//flame constructor 
Flame::Flame(Material* _material, unsigned int* _deltaTime)
{
	m_deltaTime = _deltaTime;
	m_material = _material;
	//generate a VAO index
	m_VAO = 0;
	glGenVertexArrays( 1, &m_VAO );
	//generate a VBO index
	m_posBuffer = 0;
	glGenBuffers(1, &m_posBuffer);
	//bind the buffer and push initial data to it
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUMBEROFPARTICLES * 3, &m_positions[0], GL_STATIC_DRAW);
	m_numParticles = 0;
	//create the first particle
	m_particles.push_back(new Particle);
}

//flame deconstructor
Flame::~Flame()
{
	//delete all particles
	for(unsigned int i = 0; i < m_particles.size(); i++)
	{
		delete m_particles[i];
	}
	//delete the VAO
	glDeleteVertexArrays(1,&m_VAO);
}

//update function to update particles and regenerate KDTree
void Flame::Update()
{
	float maxDist = 100000.0f;
	//fill positions array with -5 in order to not show any unupdated particles above the ground
	std::fill_n(m_positions,NUMBEROFPARTICLES*3,-5);
	//if it won't go over the maximum number of particles add new particles each frame
	if (m_particles.size() < NUMBEROFPARTICLES - 6)
	{
		for (int i = 0; i < 6; i++)
		{
			m_particles.push_back(new Particle);
		}
	}
	//generate a new KDTree of all the particles
	m_root = new KDTree(2,m_particles);
	//for every particle
	for (unsigned int i = 0; i < m_particles.size(); i++)
	{
		//zero the vector of particles within range
		m_particlesInRange.clear();
		//reset the maximum distance used in the FindParticlesInRange() function
		maxDist = 100000.0f;
		//find the particles within a range of 0.3 units
		m_root->FindParticlesInRange(0.3f,m_particles[i]->GetPos(),m_particlesInRange,&maxDist);
		//update the particle
		m_particles[i]->Update(m_particlesInRange, *m_deltaTime);
		//if the particle has reached the end of its lifetime delete it and step back a particle in order to not miss any out
		if (m_particles[i]->GetLifetime() < 0)
		{
			delete m_particles[i];
			m_particles.erase(m_particles.begin()+i);
			i--;
		}
		//otherwise push it's position to the positions array
		else
		{
			m_positions[(i*3)] = m_particles[i]->GetPos().x;
			m_positions[(i*3)+1] = m_particles[i]->GetPos().y;
			m_positions[(i*3)+2] = m_particles[i]->GetPos().z;
		}
	}
	//delete the tree
	m_root->DestroyTree();
	delete m_root;
	//bind the array and buffer object
	glBindVertexArray( m_VAO );
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
	//use glBufferSubData to stream position data to the graphics card
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * NUMBEROFPARTICLES * 3, &m_positions[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	//unbind the VAO
	glBindVertexArray(0);
}

void Flame::Draw()
{
	//disable the depth mask in order to stop depth writes and thus blending artifacts while keeping the flame from drawing over other objects
	glDepthMask(false);
	//ready the shader for drawing
	m_material->ReadyForDraw(glm::mat4(1.0f));
	//bind the VAO and draw it
	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, NUMBEROFPARTICLES);
	//unbind appropriate data
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	//re-enable the depth mask
	glDepthMask(true);
}

#pragma endregion