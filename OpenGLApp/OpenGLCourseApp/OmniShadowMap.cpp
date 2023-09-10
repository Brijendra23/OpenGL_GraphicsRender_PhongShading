#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap():ShadowMap()
{

}

bool OmniShadowMap::Init(unsigned int width,unsigned int height)
{
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}//function iterates over all six faces creating texture for each face instead of writing all faces textures individually we use i to iterate
	//overall the faces texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//reprents the z axissince a 3d

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);//since the texture is cube map so not using the 2d texture
	//the framebufffer has no color or anything just thw depth values

	//we are not drawwing anything to the frame buffer only the depth values are being stored
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

void OmniShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);//drawing to the offscreen buffer
}

void OmniShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);//which texture unit to attach
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}


OmniShadowMap::~OmniShadowMap()
{
}
