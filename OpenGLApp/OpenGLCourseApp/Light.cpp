#include "Light.h"


Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);//so basically this defines the color of the light so it shows thw coloue of values that would be reflected when it falls on th object 
	// white light or max values will show max value colour since the colour is already shown by texture 
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;

}



Light::~Light()
{

}



