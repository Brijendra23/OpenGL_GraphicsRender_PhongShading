#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include"Material.h"
#include"CommonValues.h"
#include"PointLight.h"
#include "SpotLight.h"
#include"Model.h"


GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;


const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Shader directionalShadowShader;
Shader omniShadowShader;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight  spotLights[MAX_POINT_LIGHTS];
unsigned int spotLightCount = 0;
unsigned int pointLightCount = 0;


Material shinyMaterial;
Material dullMaterial;

//Model spaceShip;
Model blackHawk;
Model airplane;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

//function for calculating the average normal for every vertices passed
void CalcAverageNormal(unsigned int *indices,unsigned int indicesCount,GLfloat *vertices,unsigned int verticeCount, 
						unsigned int vLength,unsigned int normalOffset )
{

	for (size_t i = 0; i < indicesCount; i += 3) 
	{
		// this will get us the every vector in the indices
		unsigned int in0 = indices[i] * vLength;//this will get us the first indices of every vector the x value
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);//normalizing the normal

		in0 += normalOffset; in1 += normalOffset;in2 += normalOffset;//since we need the 6th value representing the normal in vertices so we set offset
		//taking the average normal for each vertices
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;


		
	}

	//normalizing the normal values in vertices

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = (i * vLength) + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects() 
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v      Normal
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,	 0.0f,0.0f,0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,	 0.0f,0.0f,0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,  0.0f,0.0f,0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,  0.0f,0.0f,0.0f
	};

	unsigned int floorIndices[]
	{
		0,2,1,
		1,2,3
	};


	GLfloat floorVertices[] = {
		//		x      y      z				u	  v      Normal
				-10.0f, 0.0f, -10.0f,		0.0f,0.0f,	 0.0f,-1.0f,0.0f,
				10.0f, 0.0f, -10.0f,		10.0f,0.0f,	 0.0f,-1.0f,0.0f,
				-10.0f, 0.0f,10.0f,			0.0f,10.0f,  0.0f,-1.0f,0.0f,
				10.0f, 0.0f, 10.0f,			10.0f,10.0f, 0.0f,-1.0f,0.0f
	};

	CalcAverageNormal(indices, 12, vertices, 32, 8, 5);//creating average normal vector that helps in interpolation and more smooth lines

	/*Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2); */

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");

	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert","Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");

}
void RenderScene()
{
	glm::mat4 model;

	/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();*/

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-8.0f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackHawk.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	airplane.RenderModel();


}
void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());//matching the viewport with the framebuffer of shadowmap we created

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();

	directionalShadowShader.SetDirectonalLightTransform(&light->CalcLightTransform());

	directionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());//matching the viewport with the framebuffer of shadowmap we created

	

	omniShadowShader.UseShader();
	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();


	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);

	glUniform1f(uniformFarPlane, light->GetFarPlane());

	omniShadowShader.SetOmniLightMatrices(light->CalculateLightTransform());
	omniShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void RenderPass(glm::mat4 projectionMatrix,glm::mat4 viewMatrix)
{
	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();
	uniformEyePosition = shaderList[0].GetEyePositonLocation();

	glViewport(0, 0, 1366, 768);
	// Clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);


	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount,3,0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount,3+pointLightCount,pointLightCount); 
	shaderList[0].SetDirectonalLightTransform(&mainLight.CalcLightTransform());
	

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);



	

	glm::vec3 lowerCamera = camera.GetCameraPosition();
	lowerCamera.y -= 0.3f;
	spotLights[0].SetFlash(lowerCamera, camera.getCameraDirection());
	
	shaderList[0].Validate();
	RenderScene();

}



int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();


	shinyMaterial = Material(1.0f, 32.0f);//shine goes multiple square of 2
	dullMaterial = Material(0.3f, 4.0f);


	blackHawk = Model();
	blackHawk.LoadModel("Models/Giant Monster Fish.obj");

	airplane = Model();
	airplane.LoadModel("Models/Intergalactic_Spaceship-(Wavefront).obj");

	mainLight = DirectionalLight(	2048,2048,
									1.0f, 1.0f, 1.0f,
									0.0f, 0.5f,
									0.0f, -15.0f, -10.0f);
	
	pointLights[0] = PointLight(1024, 1024,
								0.1f, 100.0f,
								0.0f, 1.0f, 0.0f,
								0.0f, 0.4f,
								-2.0f, 2.0f, 0.0f,
								0.3f, 0.01f, 0.01f);
	pointLightCount++;

	pointLights[1] = PointLight(1024, 1024,
								0.1f, 100.0f,
								0.0f, 0.0f, 1.0f,
								0.0f, 0.4f,
								2.0f, 2.0f, 0.0f,
								0.3f, 0.01f, 0.01f);
	pointLightCount++;


	
	spotLights[0] = SpotLight(1024, 1024,
								0.1f, 100.0f,
								1.0f, 1.0f, 1.0f,
								0.0f, 2.0f,
								0.0f, 0.0f, 0.0f,
								0.0f, -1.0f, 0.0f,
								1.0f, 0.0f, 0.0f,
								20.0f);
	spotLightCount++;




	
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);



	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		if (mainWindow.getsKeys()[GLFW_KEY_L])
		{
			spotLights[0].Toggle();
			mainWindow.getsKeys()[GLFW_KEY_L] = false;
		}

		
		
		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}


		RenderPass(projection,camera.calculateViewMatrix());


		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}