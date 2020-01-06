#version 420

//uniform mat4 MVP;
uniform mat4 matModel;						// Model or World 
uniform mat4 matModelInverseTranspose;		// For normal calculation
uniform mat4 matView; 						// View or camera
uniform mat4 matProj;						// Projection transform

in vec4 vColour;				
in vec4 vPosition;				
in vec4 vNormal;				

out vec4 fColour;	
out vec4 fVertWorldLocation;
out vec4 fNormal;


void main()
{

	
    mat4 matMVP = matProj * matView * matModel;
	
	gl_Position = matMVP * vec4(vPosition.xyz, 1.0);			//Postion of the object pushed into gpu
	
	fVertWorldLocation = matModel * vec4(vPosition.xyz, 1.0);		
	
	
	
	vec3 theNormal = normalize(vNormal.xyz);
 	fNormal = matModelInverseTranspose * vec4(theNormal, 1.0f);
	


    fColour = vColour;	
	
}
