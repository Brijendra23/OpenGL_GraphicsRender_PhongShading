#include "SpotLight.h"

SpotLight::SpotLight() :PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}



SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,
						GLfloat nearPlane, GLfloat farP,
						GLfloat red, GLfloat green, GLfloat blue, 
						GLfloat aIntensity, GLfloat dIntensity, 
						GLfloat xPos, GLfloat yPos, GLfloat zPos,
						GLfloat xDir, GLfloat yDir, GLfloat zDir, 
						GLfloat con, GLfloat lin, GLfloat exp, 
						GLfloat edg):PointLight(shadowWidth,shadowHeight,nearPlane,farP,red,green,blue,aIntensity,dIntensity,xPos,yPos,zPos,con,lin,exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	edge = edg;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation,
							GLuint positionLocation, GLuint DirectionLocation,
							GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, 
							GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);//binding the colour to the ambient id in shaders
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(constantLocation, constant);
	glUniform3f(DirectionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);

}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{

}

