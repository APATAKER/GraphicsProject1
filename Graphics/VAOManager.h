#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>



#include <string>
#include <map>
#include "cMesh.h"			// data from cMesh is loaded to the Vertex array obj(into the vertex buffer
								//, vertex layout and index bufer)

struct sVertex
{
	float x, y, z, w;			// w coordinate	
	float r, g, b, a;			// a = alpha (transparency)
	float nx, ny, nz, nw;
};


struct sModelDrawInfo
{
	sModelDrawInfo();

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

										
	sVertex* pVertices;						// local (temporary array)
	
	unsigned int* pIndices;					//  Index buffer (CPU side)

};


class cVAOManager
{
public:

																	// Takes a cMesh object and copies it into the GPU (as a VOA)
	bool LoadModelIntoVAO(std::string fileName,
		cMesh& theMesh,				
		sModelDrawInfo& drawInfo,
		unsigned int shaderProgramID);

	bool FindDrawInfoByModelName(std::string filename,				//Finds the model that has been loaded into the vertex array buffer
		sModelDrawInfo& drawInfo);									// for drawing the model into the screen 

	std::string getLastError(bool bAndClear = true);

private:

	std::map< std::string /*model name*/,
		sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

};

#endif	
