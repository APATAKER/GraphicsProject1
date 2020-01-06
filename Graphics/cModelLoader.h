#ifndef _cModelLoader_HG_
#define _cModelLoader_HG_

#include <string>	

#include"cMesh.h"
class cModelLoader													// 
{
public:
	cModelLoader();			
	~cModelLoader();		

	
	
	bool LoadPlyModel(std::string filepathname,			// Loading the PLY files by passing the "path and name of the PLY file" 
		cMesh& currentMesh);								// and " the cMesh Object to store all its details"

	void MeshLoader(std::string MeshFolderPath,cModelLoader* pTheModelLoader);

	std::vector<cMesh> vecMesh;
};

#endif //_cModelLoader_HG_

