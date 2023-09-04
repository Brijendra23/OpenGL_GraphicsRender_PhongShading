#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include"DirectionalLight.h"
#include"PointLight.h"//we importing these here so instead of calling the shaders location of  various variable repeatedly we call it here only
#include"CommonValues.h"
#include"SpotLight.h"




class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositonLocation();

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;
	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		uniformEyePosition,uniformSpecularIntensity,uniformShininess;

	GLuint uniformPointLightCount;

	//for several lights we need multiple location variable so instead we create struct array to handle multiple light

	struct {
		GLuint uniformColour;//since the colour of the light same for ambient and diffuse so we make general variable for colour
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColour;//since the colour of the light same for ambient and diffuse so we make general variable for colour
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];


	GLuint uniformSpotLightCount;
	struct {
		GLuint uniformColour;//since the colour of the light same for ambient and diffuse so we make general variable for colour
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;

	} uniformSpotLight[MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
