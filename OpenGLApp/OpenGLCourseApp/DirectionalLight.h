#pragma once
#include "Light.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class DirectionalLight :
    public Light
{
public: 
    DirectionalLight();

    DirectionalLight(GLuint shadowWidth,GLuint shadowHeight,
        GLfloat red, GLfloat green, GLfloat blue,
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir);

    
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, 
        GLfloat diffuseIntensityLocation, GLfloat directionLocation);//binding the colour and intensity with the uniform used in shader
    
    glm::mat4 CalcLightTransform();
    ~DirectionalLight();

private:
    glm::vec3 direction;

};

