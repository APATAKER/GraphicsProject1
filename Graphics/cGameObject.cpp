#include"cGameObject.h"


cGameObject::cGameObject()
{
	this->scale = 0.0f;
	this->isVisible = true;

	this->isWireframe = false;
	this->debugColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->accel = glm::vec3(0.0f, 0.0f, 0.0f);
	this->inverseMass = 0.0f;	// Infinite mass
	this->physicsShapeType = UNKNOWN;

	
	this->m_uniqueID = cGameObject::next_uniqueID;
	cGameObject::next_uniqueID++;

	this->disableDepthBufferTest = false;
	this->disableDepthBufferWrite = false;


	return;
}


unsigned int cGameObject::getUniqueID(void)
{
	return this->m_uniqueID;
}

 


unsigned int cGameObject::next_uniqueID = 1000;	
