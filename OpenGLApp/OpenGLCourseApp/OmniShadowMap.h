#pragma once
#include "ShadowMap.h"
#include<GL/glew.h>
#include<stdio.h>
class OmniShadowMap :
    public ShadowMap
{
    public:
    OmniShadowMap();

    bool Init(unsigned int width, unsigned int height);

    void Write();

    void Read(GLenum textureUnit);




    ~OmniShadowMap();

};

