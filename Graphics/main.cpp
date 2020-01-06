#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<rapidXML/rapidxml.hpp>

#include"cModelLoader.h"
#include"cShaderManager.h"
#include"VAOManager.h"
#include"cGameObject.h"
#include"cLightManager.h"
#include"ProjectControlsAndCallback.h"


#define TRUE 1
#define FALSE 0

//GLOBALS
 std::vector<cGameObject*> g_vec_pGameObjects;
 std::vector<lightControls*> vecLightControls;

 int changeObj = 1;
 int changeLightObj = 0;
 int changeCameraTar = 0;

 glm::vec3 cameraEye = glm::vec3(0.0, 80.0, -80.0);
 glm::vec3 cameraTarget = glm::vec3(0.0f, 10.0, 0.0f);
 glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);




//MAP
std::map<std::string, cGameObject*> map_pGameObjects;

// Func Prototypes
void DrawObject(glm::mat4 m,
	cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager);
glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject);
bool ReadGameObjectsData(std::string filename);



///############## USER #### INPUT ############################################################

bool isShiftKeyDownByAlone(int mods);
bool isCtrlKeyDownByAlone(int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	const float MOVESPEED = 2.0f;
	const float LIGHTATTRIBSPEED = 0.001f;
	const float LIGHTQUADSPEED = 0.00001f;
	const float ROATATESPEED = 0.1f;

	if (!isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods))
	{

		if (key == GLFW_KEY_Q)
		{
			cameraEye.y -= MOVESPEED;
		}
		if (key == GLFW_KEY_E)
		{
			cameraEye.y += MOVESPEED;
		}

		if (key == GLFW_KEY_W)
		{
			cameraEye.z += MOVESPEED;
		}
		if (key == GLFW_KEY_S)
		{
			cameraEye.z -= MOVESPEED;
		}

		if (key == GLFW_KEY_A)
		{
			cameraEye.x -= MOVESPEED;
		}
		if (key == GLFW_KEY_D)
		{
			cameraEye.x += MOVESPEED;
		}
		//Changing Camera Target
		if (key == GLFW_KEY_C && action == GLFW_PRESS)
		{
			changeCameraTar++;

			if (changeCameraTar > g_vec_pGameObjects.size() - 1)
				changeCameraTar = 0;
			cameraTarget = g_vec_pGameObjects[changeCameraTar]->positionXYZ;
		}

		//Switching to next GameObject Model
		if (key == GLFW_KEY_N)
		{
		}

	}

	if (isShiftKeyDownByAlone(mods))
	{
		if (key == GLFW_KEY_Q)
		{
			vecLightControls[changeLightObj]->lightPosition.y -= MOVESPEED;
		}
		if (key == GLFW_KEY_E)
		{
			vecLightControls[changeLightObj]->lightPosition.y += MOVESPEED;
		}

		if (key == GLFW_KEY_A)
		{
			vecLightControls[changeLightObj]->lightPosition.x -= MOVESPEED;
		}
		if (key == GLFW_KEY_D)
		{
			vecLightControls[changeLightObj]->lightPosition.x += MOVESPEED;
		}

		if (key == GLFW_KEY_W)
		{
			vecLightControls[changeLightObj]->lightPosition.z -= MOVESPEED;
		}
		if (key == GLFW_KEY_S)
		{
			vecLightControls[changeLightObj]->lightPosition.z += MOVESPEED;
		}
		//Change light Object
		if (key == GLFW_KEY_N && action == GLFW_PRESS)
		{
			changeLightObj++;
			if (changeLightObj > vecLightControls.size() - 1)
				changeLightObj = 0;
		}

		//Light on/off
		if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		{
			vecLightControls[changeLightObj]->lightONOFF = (vecLightControls[changeLightObj]->lightONOFF == TRUE) ? FALSE : TRUE;

		}

		//Light attributes motion

		if (key == GLFW_KEY_O )//&& action == GLFW_PRESS						//LinearAtten 
		{
			vecLightControls[changeLightObj]->lightLinearAtten += LIGHTATTRIBSPEED;
		}
		if (key == GLFW_KEY_P )
		{
			vecLightControls[changeLightObj]->lightLinearAtten -= LIGHTATTRIBSPEED;
		}

		if (key == GLFW_KEY_K )						//QuadraticAtten
		{
			vecLightControls[changeLightObj]->lightQuadraticAtten += LIGHTQUADSPEED;
		}
		if (key == GLFW_KEY_L )
		{
			vecLightControls[changeLightObj]->lightQuadraticAtten -= LIGHTQUADSPEED;
		}

		if (key == GLFW_KEY_U)						//ConstantAtten
		{
			vecLightControls[changeLightObj]->lightConstAtten += LIGHTATTRIBSPEED;
		}
		if (key == GLFW_KEY_I)
		{
			vecLightControls[changeLightObj]->lightConstAtten -= LIGHTATTRIBSPEED;
		}


	}//if (isShiftKeyDownByAlone(mods))


	//Game Object Controls
	if (isCtrlKeyDownByAlone(mods))
	{
		//Moving Position
		if (key == GLFW_KEY_Q)
		{
			g_vec_pGameObjects[changeObj]->positionXYZ.y += MOVESPEED;
		}
		if (key == GLFW_KEY_E)
		{
			g_vec_pGameObjects[changeObj]->positionXYZ.y -= MOVESPEED;
		}

		if (key == GLFW_KEY_A)
		{
			g_vec_pGameObjects[changeObj]->positionXYZ.x += MOVESPEED;
		}
		if (key == GLFW_KEY_D)
		{
			g_vec_pGameObjects[changeObj]->positionXYZ.x -= MOVESPEED;
		}

		if (key == GLFW_KEY_W)
		{
			g_vec_pGameObjects[changeObj]->positionXYZ.z += MOVESPEED;
		}
		if (key == GLFW_KEY_S)
		{
			g_vec_pGameObjects[changeObj]->positionXYZ.z -= MOVESPEED;
		}
		//Rotation the object
		if (key == GLFW_KEY_O && action == GLFW_PRESS)
		{
			g_vec_pGameObjects[changeObj]->rotationXYZ.x += ROATATESPEED;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			g_vec_pGameObjects[changeObj]->rotationXYZ.x -= ROATATESPEED;
		}
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
		{
			g_vec_pGameObjects[changeObj]->rotationXYZ.y += ROATATESPEED;
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			g_vec_pGameObjects[changeObj]->rotationXYZ.y -= ROATATESPEED;
		}
		if (key == GLFW_KEY_U && action == GLFW_PRESS)
		{
			g_vec_pGameObjects[changeObj]->rotationXYZ.z += ROATATESPEED;
		}
		if (key == GLFW_KEY_I && action == GLFW_PRESS)
		{
			g_vec_pGameObjects[changeObj]->rotationXYZ.z -= ROATATESPEED;
		}

		//Scale the object
		if (key == GLFW_KEY_H)
		{
			g_vec_pGameObjects[changeObj]->scale += 1.0f;
		}
		if (key == GLFW_KEY_J)
		{
			g_vec_pGameObjects[changeObj]->scale -= 1.0f;
		}

		//Changing to Next GameObject
		if (key == GLFW_KEY_N && action == GLFW_PRESS)
		{
			changeObj++;
			if (changeObj > (g_vec_pGameObjects.size() - 1))
				changeObj = 1;
		}

		//Change to wireframe/ solid
		if (key == GLFW_KEY_V && action == GLFW_PRESS)
		{
			g_vec_pGameObjects[changeObj]->isWireframe = (g_vec_pGameObjects[changeObj]->isWireframe == true) ? false : true;
		}
		if (key == GLFW_KEY_T && action == GLFW_PRESS)
		{
			g_vec_pGameObjects[0]->isWireframe = (g_vec_pGameObjects[0]->isWireframe == true) ? false : true;
		}
	}


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}
bool isShiftKeyDownByAlone(int mods)
{
	if (mods == GLFW_MOD_SHIFT)
	{

		return true;
	}

	return false;
}
bool isCtrlKeyDownByAlone(int mods)
{
	if (mods == GLFW_MOD_CONTROL)
	{
		return true;
	}
	return false;
}

///############## USER #### INPUT ############################################################



int main(int argc, char** argv)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1280, 720, "Graphics_Project_1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);


	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	///######################## MODEL #### LOADING ##### STARTS ### HERE ##########################################
	cModelLoader* pTheModelLoader = new cModelLoader();

	// Models Loaded here

	std::string MeshFilePath ="config/cMeshFilePaths.txt";
	pTheModelLoader->MeshLoader(MeshFilePath, pTheModelLoader);

	///######################## MODEL #### LOADING ##### ENDS ### HERE ##########################################


	///######################## SHADER #### LOADING ## STARTS ### HERE #############################################
	cShaderManager* pTheShaderManager = new cShaderManager();

	cShaderManager::cShader vertexShad;
	vertexShad.fileName = "assets/shaders/vertexShader01.glsl";

	cShaderManager::cShader fragShader;
	fragShader.fileName = "assets/shaders/fragmentShader01.glsl";

	if (!pTheShaderManager->createProgramFromFile("SimpleShader", vertexShad, fragShader))  
	{
		std::cout << "Error: didn't compile the shader" << std::endl;
		std::cout << pTheShaderManager->getLastError();
		return -1;
	}
	GLuint shaderProgID = pTheShaderManager->getIDFromFriendlyName("SimpleShader");
	///######################## SHADER #### LOADING ## ENDS ### HERE #############################################


	//##### MODELS ### LOADING ### INTO ### VERTEX ### ARRAY ### OBJECT #### (DATA PUSHED INTO SHADER CODE PART)###########

	cVAOManager* pTheVAOManager = new cVAOManager();

	int iterator=0;
	while (iterator != pTheModelLoader->vecMesh.size())
	{
		sModelDrawInfo meshDrawinfo;
		pTheVAOManager->LoadModelIntoVAO(pTheModelLoader->vecMesh[iterator].meshname, pTheModelLoader->vecMesh[iterator], meshDrawinfo, shaderProgID);
		iterator++;
	}


	//##### MODELS ### LOADING ### INTO ### VERTEX ### ARRAY ### OBJECT #### (DATA PUSHED INTO SHADER CODE PART)###########




	//##### GAME ### OBJECTS ### TO ### CREATED ### HERE ##################################################################

	ReadGameObjectsData("config/scene.txt");


	//##### GAME ### OBJECTS ### TO ### CREATED ### HERE ##################################################################

	

	//cameraTarget = pTerrain->positionXYZ + glm::vec3(0.0f,10.0f,0.0f);
	cameraTarget = g_vec_pGameObjects[0]->positionXYZ + glm::vec3(0.0f, 10.0f, 0.0f);
	cameraEye = glm::vec3(78.0f,84.0f,70.0f);





	//##### LIGHT ### OBJECTS ### CREATED ### HERE ########################################################################
	lightControls* Light0 = new lightControls();		//4pillar above

	Light0->lightID = 0;
	Light0->lightPosition = glm::vec3(30,312,38);
	Light0->lightLinearAtten = -0.031f;;
	Light0->lightQuadraticAtten = 0.0001801f;
	Light0->lightConstAtten= 0.0000001f;
	Light0->lightONOFF = TRUE;

	lightControls* Light1 = new lightControls();		//pumpkin

	Light1->lightID = 1;
	//Light1->lightPosition = glm::vec3(pPumpkin->positionXYZ) + glm::vec3(0.0f,5.0f,0.0f);
	Light1->lightPosition = glm::vec3(g_vec_pGameObjects[5]->positionXYZ) + glm::vec3(0.0f, 5.0f, 0.0f);
	Light1->lightLinearAtten = -0.271f;
	Light1->lightQuadraticAtten = 0.0507f;
	Light1->lightConstAtten = 0.0000001f;
	Light1->lightONOFF = TRUE;

	lightControls* Light2 = new lightControls();		//pillar1

	Light2->lightID = 2;
	Light2->lightPosition = glm::vec3(g_vec_pGameObjects[1]->positionXYZ) + glm::vec3(0.0f, 1.0f, 0.0f);
	Light2->lightLinearAtten = 0.011f;
	Light2->lightQuadraticAtten = -0.0000298998f;
	Light2->lightConstAtten = 0.0000001f;
	Light2->lightONOFF = FALSE;

	lightControls* Light3 = new lightControls();		//pillar2

	Light3->lightID = 3;
	Light3->lightPosition = glm::vec3(g_vec_pGameObjects[2]->positionXYZ) + glm::vec3(0.0f, 1.0f, 0.0f);
	Light3->lightLinearAtten = 0.011f;;
	Light3->lightQuadraticAtten = -0.0000298998f;
	Light3->lightConstAtten = 0.0000001f;
	Light3->lightONOFF = TRUE;

	lightControls* Light4 = new lightControls();		//pillar3

	Light4->lightID = 4;
	Light4->lightPosition = glm::vec3(g_vec_pGameObjects[3]->positionXYZ) + glm::vec3(0.0f, 1.0f, 0.0f);
	Light4->lightLinearAtten = 0.011f;
	Light4->lightQuadraticAtten = -0.0000298998f;
	Light4->lightConstAtten = 0.0000001f;
	Light4->lightONOFF = TRUE;

	lightControls* Light5 = new lightControls();		//pillar4

	Light5->lightID = 5;
	Light5->lightPosition = glm::vec3(g_vec_pGameObjects[4]->positionXYZ) + glm::vec3(0.0f, 1.0f, 0.0f);
	Light5->lightLinearAtten = 0.011f;;
	Light5->lightQuadraticAtten = -0.0000298998f;
	Light5->lightConstAtten = 0.0000001f;
	Light5->lightONOFF = TRUE ;

	lightControls* Light6 = new lightControls();		//skeleton eyes

	Light6->lightID = 6;
	Light6->lightPosition = glm::vec3(g_vec_pGameObjects[8]->positionXYZ) + glm::vec3(0.0f, 1.0f, 0.0f);
	Light6->lightLinearAtten = 0.049f;;
	Light6->lightQuadraticAtten = -0.0019999f;
	Light6->lightConstAtten = 0.0000001f;
	Light6->lightONOFF = FALSE;

	lightControls* Light7 = new lightControls();

	Light7->lightID = 7;
	Light7->lightPosition = glm::vec3(g_vec_pGameObjects[6]->positionXYZ) + glm::vec3(0.0f, 1.0f, 0.0f);
	Light7->lightLinearAtten = 0.1f;;
	Light7->lightQuadraticAtten = 0.0004601f;
	Light7->lightConstAtten = 0.0000001f;
	Light7->lightONOFF = FALSE;
	
	vecLightControls.push_back(Light0);
	vecLightControls.push_back(Light1);
	vecLightControls.push_back(Light2);
	vecLightControls.push_back(Light3);
	vecLightControls.push_back(Light4);
	vecLightControls.push_back(Light5);
	vecLightControls.push_back(Light6);
	vecLightControls.push_back(Light7);
	

	//std::cout << vecLightControls.size() << std::endl;
	//##### LIGHT ### OBJECTS ### CREATED ### HERE ########################################################################


	int lightIndex=0;
	// While Loop Start

	while (!glfwWindowShouldClose(window))
	{



		glUseProgram(shaderProgID);

		float ratio;
		int width, height;
		glm::mat4 p, v;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;


		p = glm::perspective(0.6f,
			ratio,
			0.1f,
			10000.0f);


		v = glm::mat4(1.0f);
		v = glm::lookAt(cameraEye,
			cameraTarget,
			upVector);


		glViewport(0, 0, width, height);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		GLint L_0_position = glGetUniformLocation(shaderProgID, "theLights[0].position");
		GLint L_0_diffuse = glGetUniformLocation(shaderProgID, "theLights[0].diffuse");
		GLint L_0_specular = glGetUniformLocation(shaderProgID, "theLights[0].specular");
		GLint L_0_atten = glGetUniformLocation(shaderProgID, "theLights[0].atten");
		GLint L_0_direction = glGetUniformLocation(shaderProgID, "theLights[0].direction");
		GLint L_0_param1 = glGetUniformLocation(shaderProgID, "theLights[0].param1");
		GLint L_0_param2 = glGetUniformLocation(shaderProgID, "theLights[0].param2");

		GLint L_1_position = glGetUniformLocation(shaderProgID,  "theLights[1].position");
		GLint L_1_diffuse = glGetUniformLocation(shaderProgID,	 "theLights[1].diffuse");
		GLint L_1_specular = glGetUniformLocation(shaderProgID,  "theLights[1].specular");
		GLint L_1_atten = glGetUniformLocation(shaderProgID,	 "theLights[1].atten");
		GLint L_1_direction = glGetUniformLocation(shaderProgID, "theLights[1].direction");
		GLint L_1_param1 = glGetUniformLocation(shaderProgID,	 "theLights[1].param1");
		GLint L_1_param2 = glGetUniformLocation(shaderProgID,	 "theLights[1].param2");

		GLint L_2_position = glGetUniformLocation(shaderProgID, "theLights[2].position");
		GLint L_2_diffuse = glGetUniformLocation(shaderProgID, "theLights[2].diffuse");
		GLint L_2_specular = glGetUniformLocation(shaderProgID, "theLights[2].specular");
		GLint L_2_atten = glGetUniformLocation(shaderProgID, "theLights[2].atten");
		GLint L_2_direction = glGetUniformLocation(shaderProgID, "theLights[2].direction");
		GLint L_2_param1 = glGetUniformLocation(shaderProgID, "theLights[2].param1");
		GLint L_2_param2 = glGetUniformLocation(shaderProgID, "theLights[2].param2");

		GLint L_3_position = glGetUniformLocation(shaderProgID, "theLights[3].position");
		GLint L_3_diffuse = glGetUniformLocation(shaderProgID, "theLights[3].diffuse");
		GLint L_3_specular = glGetUniformLocation(shaderProgID, "theLights[3].specular");
		GLint L_3_atten = glGetUniformLocation(shaderProgID, "theLights[3].atten");
		GLint L_3_direction = glGetUniformLocation(shaderProgID, "theLights[3].direction");
		GLint L_3_param1 = glGetUniformLocation(shaderProgID, "theLights[3].param1");
		GLint L_3_param2 = glGetUniformLocation(shaderProgID, "theLights[3].param2");

		GLint L_4_position = glGetUniformLocation(shaderProgID, "theLights[4].position");
		GLint L_4_diffuse = glGetUniformLocation(shaderProgID, "theLights[4].diffuse");
		GLint L_4_specular = glGetUniformLocation(shaderProgID, "theLights[4].specular");
		GLint L_4_atten = glGetUniformLocation(shaderProgID, "theLights[4].atten");
		GLint L_4_direction = glGetUniformLocation(shaderProgID, "theLights[4].direction");
		GLint L_4_param1 = glGetUniformLocation(shaderProgID, "theLights[4].param1");
		GLint L_4_param2 = glGetUniformLocation(shaderProgID, "theLights[4].param2");

		GLint L_5_position = glGetUniformLocation(shaderProgID, "theLights[5].position");
		GLint L_5_diffuse = glGetUniformLocation(shaderProgID, "theLights[5].diffuse");
		GLint L_5_specular = glGetUniformLocation(shaderProgID, "theLights[5].specular");
		GLint L_5_atten = glGetUniformLocation(shaderProgID, "theLights[5].atten");
		GLint L_5_direction = glGetUniformLocation(shaderProgID, "theLights[5].direction");
		GLint L_5_param1 = glGetUniformLocation(shaderProgID, "theLights[5].param1");
		GLint L_5_param2 = glGetUniformLocation(shaderProgID, "theLights[5].param2");

		GLint L_6_position = glGetUniformLocation(shaderProgID, "theLights[6].position");
		GLint L_6_diffuse = glGetUniformLocation(shaderProgID, "theLights[6].diffuse");
		GLint L_6_specular = glGetUniformLocation(shaderProgID, "theLights[6].specular");
		GLint L_6_atten = glGetUniformLocation(shaderProgID, "theLights[6].atten");
		GLint L_6_direction = glGetUniformLocation(shaderProgID, "theLights[6].direction");
		GLint L_6_param1 = glGetUniformLocation(shaderProgID, "theLights[6].param1");
		GLint L_6_param2 = glGetUniformLocation(shaderProgID, "theLights[6].param2");

		GLint L_7_position = glGetUniformLocation(shaderProgID, "theLights[7].position");
		GLint L_7_diffuse = glGetUniformLocation(shaderProgID, "theLights[7].diffuse");
		GLint L_7_specular = glGetUniformLocation(shaderProgID, "theLights[7].specular");
		GLint L_7_atten = glGetUniformLocation(shaderProgID, "theLights[7].atten");
		GLint L_7_direction = glGetUniformLocation(shaderProgID, "theLights[7].direction");
		GLint L_7_param1 = glGetUniformLocation(shaderProgID, "theLights[7].param1");
		GLint L_7_param2 = glGetUniformLocation(shaderProgID, "theLights[7].param2");





		glUniform4f(L_0_position,															//LIght 0  Above 4 Pillars
			Light0->lightPosition.x,
			Light0->lightPosition.y,
			Light0->lightPosition.z,
			1.0f);
		glUniform4f(L_0_diffuse, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_0_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_0_atten, 0.0f,						 // constant attenuation
			Light0->lightLinearAtten,							 // Linear 
			Light0->lightQuadraticAtten,						// Quadratic 
			1000000.0f);									// Distance cut off

		// Point light:
		glUniform4f(L_0_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);

		glUniform4f(L_0_param2, Light0->lightONOFF /*Light is on*/, 0.0f, 0.0f, 1.0f);



						
		glUniform4f(L_1_position,															//Light 1 - Pumpkin 
			Light1->lightPosition.x,
			Light1->lightPosition.y,
			Light1->lightPosition.z,
			1.0f);
		glUniform4f(L_1_diffuse, 1.0f, 1.0f, 0.0f, 1.0f);	// Yellow
		glUniform4f(L_1_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_1_atten, 0.0f,						
			Light1->lightLinearAtten,						
			Light1->lightQuadraticAtten,					
			1000000.0f);									

		// Point light:
		glUniform4f(L_1_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);

		glUniform4f(L_1_param2, Light1->lightONOFF /*Light is on*/, 0.0f, 0.0f, 1.0f);



		glUniform4f(L_2_position,														//Light 2 - at pillar 1
			Light2->lightPosition.x,
			Light2->lightPosition.y,
			Light2->lightPosition.z,
			1.0f);
		glUniform4f(L_2_diffuse, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_2_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_2_atten, 0.0f,						
			Light2->lightLinearAtten,						
			Light2->lightQuadraticAtten,					
			1000000.0f);									

		// Point light:
		glUniform4f(L_2_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
		glUniform4f(L_2_param2, Light2->lightONOFF /*Light is on*/, 0.0f, 0.0f, 1.0f);


		glUniform4f(L_3_position,														//Light 3 - at pillar 2
			Light3->lightPosition.x,
			Light3->lightPosition.y,
			Light3->lightPosition.z,
			1.0f);
		glUniform4f(L_3_diffuse, 0.0f, 0.0f, 1.0f, 1.0f);	// Blue
		glUniform4f(L_3_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_3_atten, 0.0f,						
			Light3->lightLinearAtten,						
			Light3->lightQuadraticAtten,					
			1000000.0f);									

		// Point light:
		glUniform4f(L_3_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
		glUniform4f(L_3_param2, Light3->lightONOFF /*Light is on*/, 0.0f, 0.0f, 1.0f);


		glUniform4f(L_4_position,														//Light 4 - at pillar 3
			Light4->lightPosition.x,
			Light4->lightPosition.y,
			Light4->lightPosition.z,
			1.0f);
		glUniform4f(L_4_diffuse, 0.0f, 1.0f, 0.0f, 1.0f);	// Green
		glUniform4f(L_4_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_4_atten, 0.0f,
			Light4->lightLinearAtten,
			Light4->lightQuadraticAtten,
			1000000.0f);

		// Point light:
		glUniform4f(L_4_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
		glUniform4f(L_4_param2, Light4->lightONOFF /*Light is on*/, 0.0f, 0.0f, 1.0f);


		glUniform4f(L_5_position,														//Light 5 - at pillar 4
			Light5->lightPosition.x,
			Light5->lightPosition.y,
			Light5->lightPosition.z,
			1.0f);
		glUniform4f(L_5_diffuse, 1.0f, 0.0f, 0.0f, 1.0f);	// Red
		glUniform4f(L_5_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_5_atten, 0.0f,
			Light5->lightLinearAtten,
			Light5->lightQuadraticAtten,
			1000000.0f);

		// Point light:
		glUniform4f(L_5_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
		glUniform4f(L_5_param2, Light5->lightONOFF /*Light is on*/, 0.0f, 0.0f, 1.0f);


		glUniform4f(L_6_position,														//Light 6 - at skeleton eyes
			Light6->lightPosition.x,
			Light6->lightPosition.y,
			Light6->lightPosition.z,
			1.0f);
		glUniform4f(L_6_diffuse, 1.0f, 0.0f, 0.0f, 1.0f);	// White
		glUniform4f(L_6_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_6_atten, 0.0f,
			Light6->lightLinearAtten,
			Light6->lightQuadraticAtten,
			1000000.0f);

		// Point light:
		glUniform4f(L_6_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
		glUniform4f(L_6_param2, Light6->lightONOFF /*Light is on*/, 0.0f, 0.0f, 1.0f);


		glUniform4f(L_7_position,														//Light 7 - at URN
			Light7->lightPosition.x,
			Light7->lightPosition.y,
			Light7->lightPosition.z,
			1.0f);
		glUniform4f(L_7_diffuse, 0.3176f, 1.0f, 0.0509f, 1.0f);	// White
		glUniform4f(L_7_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_7_atten, 0.0f,
			Light7->lightLinearAtten,
			Light7->lightQuadraticAtten,
			1000000.0f);

		// Point light:
		glUniform4f(L_7_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
		glUniform4f(L_7_param2, Light7->lightONOFF /*Light is on*/, 0.0f, 0.0f, 1.0f);

		// Also set the position of my "eye" (the camera)
		GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");

		glUniform4f(eyeLocation_UL,
			cameraEye.x, cameraEye.y, cameraEye.z, 1.0f);


		std::stringstream ssTitle;
		ssTitle
			<< "Light ID: "
			<< vecLightControls[changeLightObj]->lightID<<" = "
			<< vecLightControls[changeLightObj]->lightPosition.x << ", "
			<< vecLightControls[changeLightObj]->lightPosition.y << ", "
			<< vecLightControls[changeLightObj]->lightPosition.z
			<< " isOn/Off: "
			<<vecLightControls[changeLightObj]->lightONOFF
			<< " Atten: "
			<< vecLightControls[changeLightObj]->lightConstAtten << " : "
			<< vecLightControls[changeLightObj]->lightLinearAtten << " : "
			<< vecLightControls[changeLightObj]->lightQuadraticAtten
			<< " Cam Pos: "
			<< cameraEye.x << ", "
			<< cameraEye.y << ", "
			<< cameraEye.z
			<< "CurObj Name: " << g_vec_pGameObjects[changeObj]->friendlyName
			<< " CurrObj Pos: "
			<< g_vec_pGameObjects[changeObj]->positionXYZ.x << ", "
			<< g_vec_pGameObjects[changeObj]->positionXYZ.y << ", "
			<< g_vec_pGameObjects[changeObj]->positionXYZ.z << ", "
			<< " CurrObj Rot: "
			<< g_vec_pGameObjects[changeObj]->rotationXYZ.x << ", "
			<< g_vec_pGameObjects[changeObj]->rotationXYZ.y << ", "
			<< g_vec_pGameObjects[changeObj]->rotationXYZ.z << ", ";
		
		
		glfwSetWindowTitle(window, ssTitle.str().c_str());


		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));




		for (int index = 0; index != g_vec_pGameObjects.size(); index++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);

			cGameObject* pCurrentObject = g_vec_pGameObjects[index];

			DrawObject(matModel, pCurrentObject,
				shaderProgID, pTheVAOManager);

		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//While Loop Ends







	glfwDestroyWindow(window);
	glfwTerminate();

	delete pTheModelLoader;

	exit(EXIT_SUCCESS);

	return 0;
}


// Draw OBject func starts

void DrawObject(glm::mat4 m,
	cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager)
{
	m = calculateWorldMatrix(pCurrentObject);

	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");
	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));

	GLint matModelIT_UL = glGetUniformLocation(shaderProgID, "matModelInverseTranspose");
	glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(m));
	glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

	GLint newColour_location = glGetUniformLocation(shaderProgID, "newColour");
	glUniform4f(newColour_location,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b,1.0f);

	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	glUniform4f(diffuseColour_UL,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b,
		pCurrentObject->objectColourRGBA.a);

	GLint specularColour_UL = glGetUniformLocation(shaderProgID, "specularColour");
	glUniform4f(specularColour_UL,
		1.0f,	// R
		1.0f,	// G
		1.0f,	// B
		1000.0f);	// Specular "power" (how shinny the object is)
					// 1.0 to really big (10000.0f)


	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");

	if (pCurrentObject->isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		glUniform4f(debugColour_UL,
			pCurrentObject->debugColour.r,
			pCurrentObject->debugColour.g,
			pCurrentObject->debugColour.b,
			pCurrentObject->debugColour.a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else
	{	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
	}
	//glPointSize(15.0f);

	if (pCurrentObject->disableDepthBufferTest)
	{
		glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF
	}
	else
	{
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
	}

	if (pCurrentObject->disableDepthBufferWrite)
	{
		glDisable(GL_DEPTH);						// depth buffer ON
	}
	else
	{
		glEnable(GL_DEPTH);								// depth buffer OFF
	}

	sModelDrawInfo drawInfo;

	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}

	return;

}

// Draw Object func ends

// Calculate world mat func starts

glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject)
{

	glm::mat4 matWorld = glm::mat4(1.0f);


	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
		= glm::translate(glm::mat4(1.0f),
			glm::vec3(pCurrentObject->positionXYZ.x,
				pCurrentObject->positionXYZ.y,
				pCurrentObject->positionXYZ.z));
	matWorld = matWorld * matTrans;
	// ******* TRANSLATION TRANSFORM *********



	// ******* ROTATION TRANSFORM *********
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.z,											// Angle 
		glm::vec3(0.0f, 0.0f, 1.0f));
	matWorld = matWorld * rotateZ;

	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.y,											// Angle 
		glm::vec3(0.0f, 1.0f, 0.0f));
	matWorld = matWorld * rotateY;

	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
		pCurrentObject->rotationXYZ.x,											// Angle 
		glm::vec3(1.0f, 0.0f, 0.0f));
	matWorld = matWorld * rotateX;
	// ******* ROTATION TRANSFORM *********



	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(pCurrentObject->scale,
			pCurrentObject->scale,
			pCurrentObject->scale));
	matWorld = matWorld * scale;
	// ******* SCALE TRANSFORM *********


	return matWorld;
}

// Calculate world mat func ends


// one func to load all game Objects starts here
bool ReadGameObjectsData(std::string filename)
{
	std::ifstream GameObjectData;

	GameObjectData.open(filename);
	if (!GameObjectData.is_open()) {
		printf("Unable to open file\n");
		system("pause");
		return false;
	}
	

	while (true)
	{

		cGameObject* currentGameObject = new cGameObject();
		std::string temp;


		GameObjectData >> temp;

		if (temp == "GameObject")
		{
			GameObjectData >> currentGameObject->meshName;
		}
		else
		{
			break;						// if temp is not at "GameObject" now , then the loop has reached the "end"
		}

		GameObjectData >> temp;

		if (temp == "FriendlyName")
		{
			GameObjectData >> currentGameObject->friendlyName;
		}


		GameObjectData >> temp;

		if (temp == "Position")
		{
			GameObjectData >> currentGameObject->positionXYZ.x >> currentGameObject->positionXYZ.y >> currentGameObject->positionXYZ.z;
		}

		GameObjectData >> temp;

		if (temp == "Rotation")
		{
			GameObjectData >> currentGameObject->rotationXYZ.x >> currentGameObject->rotationXYZ.y >> currentGameObject->rotationXYZ.z;
		}

		GameObjectData >> temp;

		if (temp == "Scale")
		{
			GameObjectData >> currentGameObject->scale;
		}

		GameObjectData >> temp;

		if (temp == "Color")
		{
			GameObjectData >> currentGameObject->objectColourRGBA.x >> currentGameObject->objectColourRGBA.y >> currentGameObject->objectColourRGBA.z;
		}

		GameObjectData >> temp;


		if (temp == "Wireframe")
		{
			GameObjectData >> currentGameObject->isWireframe;
		}

		g_vec_pGameObjects.push_back(currentGameObject);

	}
	return true;
}
// one func to load all game Objects starts here