#include "Mesh.h"
#include <fstream>
#include <vector>
#include <glm.hpp>
#include <sstream>
#include <iostream>


Mesh::Mesh(std::string _fileName)
{
	//generate a VAO and load a file into it
	m_VAO = 0;
	glGenVertexArrays( 1, &m_VAO );
	m_numVertices = 0;
	LoadOBJ(_fileName);
}


Mesh::~Mesh(void)
{
	//delete the VAO
	glDeleteVertexArrays(1,&m_VAO);
}

//This function was originally written by Leigh Mcloughlin
//Tangent calculations and handling were added by myself
void Mesh::LoadOBJ(std::string _fileName)
{
	std::ifstream inputFile( _fileName );

	if( inputFile.is_open() )
	{
		std::vector<glm::vec2> rawUVData;
		std::vector<glm::vec3> rawPositionData;
		std::vector<glm::vec3> rawNormalData;		
		std::vector<glm::vec2> orderedUVData;
		std::vector<glm::vec3> orderedPositionData;
		std::vector<glm::vec3> orderedNormalData;
		std::vector<glm::vec3> tangentData;
		std::string currentLine;
		while( std::getline( inputFile, currentLine ) )
		{
			std::stringstream currentLineStream(currentLine);					
			if( !currentLine.substr(0,2).compare( 0, 2, "vt") )
			{
				std::string junk;
				float x, y;
				currentLineStream >> junk >> x >> y;
				rawUVData.push_back(glm::vec2(x,-y));//had to edit this as UVs had flipped a V axis
			}
			else if( !currentLine.substr(0,2).compare( 0, 2, "vn") )
			{
				std::string junk;
				float x, y, z;
				currentLineStream >> junk >> x >> y >> z;
				rawNormalData.push_back(glm::vec3(x,y,z));
			}
			else if( !currentLine.substr(0,2).compare( 0, 1, "v") )
			{
				std::string junk;
				float x, y, z;
				currentLineStream >> junk >> x >> y >> z;
				rawPositionData.push_back(glm::vec3(x,y,z));
			}
			else if( !currentLine.substr(0,2).compare( 0, 1,"f") )
			{
				std::string junk;
				std::string verts[4];
				currentLineStream >> junk >> verts[0] >> verts[1] >> verts[2] >> verts[3];
				if( verts[3].empty() )
				{
					for( unsigned int i = 0; i < 3; i++ )
					{
						std::stringstream currentSection(verts[i]);
						unsigned int posID = 0;
						unsigned int uvID = 0;
						unsigned int normID = 0;
						if( verts[i].find('/') == std::string::npos )
						{
							currentSection >> posID;
						}
						else if( verts[i].find("//") != std::string::npos )
						{
							char junk;
							currentSection >> posID >> junk >> junk >> normID;
						}
						else
						{
							char junk;
							currentSection >> posID >>junk >> uvID >> junk >> normID;
						}
						if( posID > 0 )
						{
							orderedPositionData.push_back( rawPositionData[posID-1] );
						}
						if( uvID > 0 )
						{
							orderedUVData.push_back( rawUVData[uvID-1] );
						}
						if( normID > 0 )
						{
							orderedNormalData.push_back( rawNormalData[normID-1] );
						}
					}
				}
				else
				{
					std::cerr<<"WARNING: This OBJ loader only works with triangles but a quad has been detected. Please triangulate your mesh."<<std::endl;
					inputFile.close();
					return;
				}
			}
		}
		inputFile.close();
		m_numVertices = orderedPositionData.size();

		if( m_numVertices > 0 )
		{
			
			glBindVertexArray( m_VAO );
			GLuint posBuffer = 0;
			glGenBuffers(1, &posBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, &orderedPositionData[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray(0);
			if( orderedNormalData.size() > 0 )
			{
				GLuint normBuffer = 0;
				glGenBuffers(1, &normBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, &orderedNormalData[0], GL_STATIC_DRAW);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
				glEnableVertexAttribArray(1);
			}
			if( orderedUVData.size() > 0 )
			{
				GLuint texBuffer = 0;
				glGenBuffers(1, &texBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 2, &orderedUVData[0], GL_STATIC_DRAW);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
				glEnableVertexAttribArray(2);

				//below here is what I added
				//tangent space calculations
				for (unsigned int i=0; i < m_numVertices; i+=3)
				{
					//find the 2 edges attached to the given vertex
					glm::vec3 edge1 = orderedPositionData[i+1] - orderedPositionData[i];
					glm::vec3 edge2 = orderedPositionData[i+2] - orderedPositionData[i];
					//find differences between Us and Vs on the two edges
					float deltaU1 = orderedUVData[i+1].x - orderedUVData[i].x;
					float deltaV1 = orderedUVData[i+1].y - orderedUVData[i].y;
					float deltaU2 = orderedUVData[i+2].x - orderedUVData[i].x;
					float deltaV2 = orderedUVData[i+2].y - orderedUVData[i].y;
					
					float scalarFraction = 1.0f/ (deltaU1*deltaV2 - deltaU2*deltaV1);

					glm::vec3 tangent;
					//set up tangent vector
					tangent.x = scalarFraction * (deltaV2*edge1.x - deltaV1*edge2.x);
					tangent.y = scalarFraction * (deltaV2*edge1.y - deltaV1*edge2.y);
					tangent.z = scalarFraction * (deltaV2*edge1.z - deltaV1*edge2.z);
					//normalize it
					glm::normalize(tangent);
					//add tangent data to all vertices used in the calculations
					tangentData.push_back(tangent);
					tangentData.push_back(tangent);
					tangentData.push_back(tangent);
				}

				//generate the tangent buffer and push the data to the graphics card
				GLuint tangentBuffer = 0;
				glGenBuffers(1,&tangentBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, &tangentData[0], GL_STATIC_DRAW);
				glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,0);
				glEnableVertexAttribArray(3);
				//end of what I added
			}
		}
	}
	else
	{
		std::cerr<<"WARNING: File not found: "<< _fileName <<std::endl;
	}
	glBindVertexArray( 0 );
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}
//end of function

void Mesh::Draw()
{
	//bind VAO and enable attributes
	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	
	//draw the mesh
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

	//unbind VAO and disable attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glBindVertexArray(0);
}