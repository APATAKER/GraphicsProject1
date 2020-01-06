#include "cModelLoader.h"

#include <iostream>			
#include<fstream>	    

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

cModelLoader::cModelLoader()			// constructor
{
	std::cout << "ModelLoader is created!" << std::endl;
	return;
}

cModelLoader::~cModelLoader()			// destructor
{
	std::cout << "ModelLoader is destroyed" << std::endl;
	return;
}


bool cModelLoader::LoadPlyModel(						
	std::string filepathname,
	cMesh& currentMesh)				
{
	//std::ifstream theFile(currentMesh.filepath.c_str());							//Opening the Ply File
	std::ifstream theFile(filepathname.c_str()); 
	if (!theFile.is_open())
	{
		return false;
	}

	// Scan the file until I get to "vertex", and stop
	std::string temp;

	theFile >> temp;
	currentMesh.meshname = temp.c_str();
	
	while (theFile >> temp)													//Scanning the PLY file
	{
		// Have it hit the word "vertex"?
		if (temp == "vertex")
		{
			break;		// Exit the while loop
		}
	}

	unsigned int numberOfVertices;
	theFile >> numberOfVertices;											//Getting the Number Of Vertices



	while (theFile >> temp)
	{
		// Have it hit the word "face"?
		if (temp == "face")
		{
			break;		// Exit the while loop
		}
	}

	unsigned int numberOfTriangles;
	theFile >> numberOfTriangles;											//Getting the Number Of Triangles

	while (theFile >> temp)
	{
		// Have it hit the word "end_header"?
		if (temp == "end_header")
		{
			break;		// Exit the while loop
		}
	}// while ( theFile >> temp ) 



																					// Getting all the vertices data
	for (unsigned int index = 0; index != numberOfVertices; index++)
	{
		
		sPlyVertexXYZ_N tempVertex;

		
		theFile >> tempVertex.x >> tempVertex.y >> tempVertex.z;					// Getting the XYZ of 1 vertex

																					
		glm::vec3 normal;															//  load the normals of XYZ of 1 vertex
		theFile >> normal.x >> normal.y >> normal.z;
		normal = glm::normalize(normal);
		tempVertex.nx = normal.x;
		tempVertex.ny = normal.y;
		tempVertex.nz = normal.z;

		
		currentMesh.vecVertices.push_back(tempVertex);
	}

	for (unsigned int index = 0; index != numberOfTriangles; index++)				//Getting all the indices data(Triangle)
	{
		
		int discardThis;
		sPlyTriangleIndex tempTriangle;

		theFile >> discardThis
			>> tempTriangle.Triangle_vert_index_1
			>> tempTriangle.Triangle_vert_index_2
			>> tempTriangle.Triangle_vert_index_3;

		// Add this triangle
		currentMesh.vecTriangleIndexs.push_back(tempTriangle);
	}


	return true;
}

void cModelLoader::MeshLoader(std::string MeshFolderPath, cModelLoader* pTheModelLoader)
{
	std::ifstream MeshFile;
	MeshFile.open(MeshFolderPath);
	if (!MeshFile) {
		printf("Unable to open file\n");
		system("pause");
		return ;
	}
	std::string line;
	while (std::getline(MeshFile, line))
	{
		if (line.length() > 0)
		{
			cMesh mesh;
			mesh.setPath(line.c_str());
			pTheModelLoader->LoadPlyModel(mesh.filepath, mesh);

			std::cout << mesh.meshname << std::endl;
			vecMesh.push_back(mesh);

		}
	}
	MeshFile.close();

}
