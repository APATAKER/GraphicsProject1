#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <vector>	
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


struct sPlyVertexXYZ_N
{
	float x, y, z;						// These are variables for the ply files to be loaded X,Y,Z and NORMALS X,Y,Z
	float nx, ny, nz;		
};

struct sPlyTriangleIndex						// These are variable for building a Triangle with the above vertices
{
	unsigned int Triangle_vert_index_1;
	unsigned int Triangle_vert_index_2;			
	unsigned int Triangle_vert_index_3;
};

class cMesh
{
public:
	cMesh() {};			
	~cMesh() {};		

	std::string filepath;
	std::string meshname;

	
	std::vector<sPlyVertexXYZ_N> vecVertices;			

																		//Created a vector for all the vertices and Triangle Indexs
	std::vector<sPlyTriangleIndex> vecTriangleIndexs;

	inline void setPath(std::string meshpath) { filepath = meshpath; }

};

#endif 
