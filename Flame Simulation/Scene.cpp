#include "Scene.h"
#define MODEL_SHADER_WITH_NORMAL 1
#define PARTICLE_SHADER 0
#define MODEL_SHADER 2

//constructor of the scene, used to generate the scene (further expansion could be done here to load a scene from a file)
Scene::Scene(void)
{
	//set the initial light position
	m_lightPosition = glm::vec4(0,0.1,0,1);
	//start the light moving up
	m_fireUp = true;
	m_rotateRight = false;
	m_rotateLeft = false;
	//create shaders
	m_logShader = new Shader(MODEL_SHADER_WITH_NORMAL);
	m_pointsShader = new Shader(PARTICLE_SHADER);
	m_grassShader = new Shader(MODEL_SHADER_WITH_NORMAL);

	//create meshes and materials and push objects to the back of the list
	m_grassMesh = new Mesh("Grass.obj");
	m_grassMat = new Material("Grass.png","Grass_normal.png", m_grassShader, &m_cameraRotation, &m_lightPosition);
	m_objects.push_back(new Object(m_grassMesh, m_grassMat,&m_deltaTime, glm::vec3(0.5,0.5,0.5), glm::vec3(0,-0.3,0), glm::vec3(0,0,0)));
	
	m_logMesh = new Mesh("Log_pine.obj");
	m_logMat = new Material("Log_pine_color.png", "Log_pine_normal.png", m_logShader, &m_cameraRotation, &m_lightPosition);

	m_particleMat = new Material("Flame_Particle.png","NO", m_pointsShader, &m_cameraRotation, &m_lightPosition);

	m_objects.push_back(new Object(m_logMesh, m_logMat,&m_deltaTime, glm::vec3(0.1f,0.1f,0.1f), glm::vec3(-3.0f,0.1,0), glm::vec3(3.1415,0,0)));
	m_objects.push_back(new Object(m_logMesh, m_logMat,&m_deltaTime, glm::vec3(0.1f,0.1f,0.1f), glm::vec3(3.0f,0.1,0), glm::vec3(3.1415,0,0)));
	m_objects.push_back(new Object(m_logMesh, m_logMat,&m_deltaTime, glm::vec3(0.1f,0.1f,0.1f), glm::vec3(0,0.1,-3.0f), glm::vec3(3.1415,1.57079632679f,0)));
	m_objects.push_back(new Object(m_logMesh, m_logMat,&m_deltaTime, glm::vec3(0.1f,0.1f,0.1f), glm::vec3(0,0.1,3.0f), glm::vec3(3.1415,1.57079632679f,0)));
	m_fires.push_back(new Flame(m_particleMat, &m_deltaTime));
	
	//set initial rotation and deltatime
	m_cameraRotation = glm::vec3(0,0,0);
	m_previousTime = SDL_GetTicks();
	m_deltaTime = 0;
}

//function to update each object in the scene and rotate the camera and light
void Scene::Update()
{
	//rotate the camera based on input
	if (m_rotateRight)
	{
		m_cameraRotation.y -= 0.4*((float)m_deltaTime / 1000);
	}
	if (m_rotateLeft)
	{
		m_cameraRotation.y += 0.4*((float)m_deltaTime / 1000);
	}

	//move the fire up or down to simulate flickering on surroundings
	if(m_fireUp)
	{
		m_lightPosition.y += 0.6*((float)m_deltaTime / 1000);
		if (m_lightPosition.y>2)
		{
			m_fireUp = false;
		}
	}
	else
	{
		m_lightPosition.y -= 0.6*((float)m_deltaTime / 1000);
		if (m_lightPosition.y<1.5)
		{
			m_fireUp = true;
		}
	}

	//loop through all objcts and fires and update them
	for( auto i : m_objects)
	{
		i->Update();
	}
	for (auto i : m_fires)
	{
		i->Update();
	}
	
}

//function to draw all objects and flames
void Scene::Draw()
{
	//clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//loop through all objects and fires and draw them
	for(auto i : m_objects)
	{
		i->Draw();
	}
	for (auto i : m_fires)
	{
		i->Draw();
	}
}