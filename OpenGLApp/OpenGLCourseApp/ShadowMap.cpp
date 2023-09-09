#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	FBO = 0;
	shadowMap = 0;

}

bool ShadowMap::Init(unsigned int width, unsigned int height)
{
	shadowHeight = height;
	shadowWidth = width;
	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);//depth component calculation of the pixel in scale 0-1
	//this creates a texture with only the depth values and combined with frame buffer to create the real texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float bColour[] = {1.0f,1.0f,1.0f,1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);//this can be used for both reading and drawing frame buffer, however there can these can be done individually
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap,0);//this function used to attach the texture map to the frame buffer that is to be drawn on the screen
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);//checking whether the linking is done properly

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error : %i\n", status);
		return false;
	}

	
	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);//drawing to the offscreen buffer
}

void ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);//which texture unit to attach
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}




ShadowMap::~ShadowMap()
{
	if (FBO) {
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}
