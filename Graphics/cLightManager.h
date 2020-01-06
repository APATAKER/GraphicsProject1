#ifndef _cLightManager_HG_
#define _cLightManager_HG_


#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
struct lightControls
{
	int lightID;
	glm::vec3 lightPosition;
	float lightLinearAtten;
	float lightQuadraticAtten;
	float lightConstAtten;

	float lightONOFF;

};

class cLightManager
{
public:
	cLightManager();
	~cLightManager();

	glm::vec4 lightPosition;
	glm::vec4 lightDiffuse;
	glm::vec4 lightSpecular;
	glm::vec4 lightAtten;
	glm::vec4 lightDirection;
	glm::vec4 lightParam1;
	glm::vec4 lightParam2;


private:

};



#endif // _cLightManager_HG
