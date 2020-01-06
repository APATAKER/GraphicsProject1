#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>


enum eShapeTypes
{
	AABB,					 
	SPHERE,					
	CAPSULE,
	PLANE,
	MESH,		
	UNKNOWN
};

class cGameObject
{
public:
	cGameObject();
	
	std::string meshName;			

	unsigned int friendlyIDNumber;
	std::string friendlyName;

	glm::vec3  positionXYZ;
	glm::vec3  rotationXYZ;
	float scale;;

	glm::mat4 matWorld;

	glm::vec4  objectColourRGBA;

	glm::vec4 diffuseColour;	
	glm::vec4 specularColour;	
								
								
								


	glm::vec3 velocity;
	glm::vec3 accel;

	float HACK_speed;
	float HACK_AngleAroundYAxis;		


	float inverseMass;	


	eShapeTypes physicsShapeType;
	
	glm::vec3 AABB_min, AABB_max;
	float SPHERE_radius;

	bool isWireframe;
	glm::vec4 debugColour;

	bool isVisible;

	bool disableDepthBufferTest;
	bool disableDepthBufferWrite;

	unsigned int getUniqueID(void);

	


private:

	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;

};

#endif


