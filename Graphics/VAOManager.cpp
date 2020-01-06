#include "VAOManager.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

#include <sstream>

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	
	this->pVertices = 0;					
	this->pIndices = 0;		


	return;
}




bool cVAOManager::LoadModelIntoVAO(
	std::string meshName,
	cMesh& theMesh,					
	sModelDrawInfo& drawInfo,
	unsigned int shaderProgramID)

{

	drawInfo.numberOfVertices = (unsigned int)theMesh.vecVertices.size();
	
	drawInfo.pVertices = new sVertex[drawInfo.numberOfVertices];				// Allocating the memory needed for total number of vertex


	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)			//Loaded Vertices from Cmesh to Vertex Array object
	{

		drawInfo.pVertices[index].x = theMesh.vecVertices[index].x;
		drawInfo.pVertices[index].y = theMesh.vecVertices[index].y;
		drawInfo.pVertices[index].z = theMesh.vecVertices[index].z;
		drawInfo.pVertices[index].w = 1.0f;		

		drawInfo.pVertices[index].r = 1.0f;
		drawInfo.pVertices[index].g = 1.0f;
		drawInfo.pVertices[index].b = 1.0f;
		drawInfo.pVertices[index].a = 1.0f;		

		drawInfo.pVertices[index].nx = theMesh.vecVertices[index].nx;
		drawInfo.pVertices[index].ny = theMesh.vecVertices[index].ny;
		drawInfo.pVertices[index].nz = theMesh.vecVertices[index].nz;
		drawInfo.pVertices[index].nw = 1.0f;		

	}

	drawInfo.numberOfTriangles = (unsigned int)theMesh.vecTriangleIndexs.size();
	drawInfo.numberOfIndices = (unsigned int)theMesh.vecTriangleIndexs.size() * 3;


	drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];			// Allocating the memory needed for total number of indices

	unsigned int indexTri = 0;
	unsigned int indexIndex = 0;
	for (; indexTri != drawInfo.numberOfTriangles; indexTri++, indexIndex += 3)				//Loaded Indices from Cmesh to Vertex Array object
	{
		drawInfo.pIndices[indexIndex + 0] = (unsigned int)theMesh.vecTriangleIndexs[indexTri].Triangle_vert_index_1;
		drawInfo.pIndices[indexIndex + 1] = (unsigned int)theMesh.vecTriangleIndexs[indexTri].Triangle_vert_index_2;
		drawInfo.pIndices[indexIndex + 2] = (unsigned int)theMesh.vecTriangleIndexs[indexTri].Triangle_vert_index_3;
	}


	drawInfo.meshName = meshName;

	//Creating the Vertex Array Object and Buffers respectively

	glGenVertexArrays(1, &(drawInfo.VAO_ID));

	glBindVertexArray(drawInfo.VAO_ID);

	

	glGenBuffers(1, &(drawInfo.VertexBufferID));				// Creating Vertex Buffer in VAO

	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);

	glBufferData(GL_ARRAY_BUFFER,								//Loading the vertices into the vertex buffer
		sizeof(sVertex) * drawInfo.numberOfVertices,				
		(GLvoid*)drawInfo.pVertices,						
		GL_STATIC_DRAW);


	glGenBuffers(1, &(drawInfo.IndexBufferID));					// Creating Index Buffer in VAO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,						// Loading the indices into the index buffer 
		sizeof(unsigned int) * drawInfo.numberOfIndices,
		(GLvoid*)drawInfo.pIndices,
		GL_STATIC_DRAW);

	
	// Setting the Uniform variable to be sent to the shader code

	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");	
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");		
	GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");		
	GLint vUV_location = glGetAttribLocation(shaderProgramID, "vUVx2");			

	// Setting the Vertex Layout Here

	glEnableVertexAttribArray(vpos_location);	
	glVertexAttribPointer(vpos_location, 4,		
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),						
		(void*)(offsetof(sVertex, x)));

	glEnableVertexAttribArray(vcol_location);	
	glVertexAttribPointer(vcol_location, 4,		
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)(offsetof(sVertex, r)));

	glEnableVertexAttribArray(vnorm_location);	
	glVertexAttribPointer(vnorm_location, 4,	
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)(offsetof(sVertex, nx)));





	// UnBind after Binding
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);


	// Storing the draw information into A map
	this->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;


	return true;
}


bool cVAOManager::FindDrawInfoByModelName(								//Finds the model that has been loaded into the vertex array buffer 
	std::string filename,												// for drawing the model into the screen 
	sModelDrawInfo& drawInfo)
{
	std::map< std::string ,
		sModelDrawInfo  >::iterator
		itDrawInfo = this->m_map_ModelName_to_VAOID.find(filename);

	if (itDrawInfo == this->m_map_ModelName_to_VAOID.end())
	{
		return false;
	}

	drawInfo = itDrawInfo->second;
	return true;
}

