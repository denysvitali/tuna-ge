#include "Texture.h"

#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF

// FreeGLUT:
#include <GL/freeglut.h>


tunage::Texture::~Texture()
{
	glDeleteTextures(1, &texId);
}

void tunage::Texture::render()
{
	if (!initialized) {
		initialized = true;
		init();
	}
	else {
		// Update texture content:
		glBindTexture(GL_TEXTURE_2D, texId);
	}
	

	
}

void tunage::Texture::loadTextureFromFile()
{
	//TODO: implement freeimage
}

void tunage::Texture::loadTexture(unsigned char * texture)
{
	this->texture = texture;
}

void tunage::Texture::setAnisotropic(bool anisotropic)
{
	this->anisotropic = anisotropic;
}

void tunage::Texture::setMipmap(bool useMipmaps)
{
	this->useMipmaps = useMipmaps;
}

void tunage::Texture::setAnisotropicLevel(int anisotropicLevel)
{
	this->anisotropicLevel = anisotropicLevel;
}

void tunage::Texture::init()
{
	if (strstr((const char *)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic"))
	{
		isAnisotropicSupported = true;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicLevel);
	}
	else
	{
		isAnisotropicSupported = false;
	}

	glGenTextures(1, &texId);
	texture = new unsigned char[256 * 256 * 3];
	
	for (int i = 0; i < 256*256*3; i += 3) {
		texture[i] = 255;
	}

	// Update texture content:
	glBindTexture(GL_TEXTURE_2D, texId);

	// Set circular coordinates:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (anisotropic && isAnisotropicSupported)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicLevel);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (!useMipmaps)
	{
		// Without mipmapping:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		/*	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				FreeImage_GetWidth(abitmap), FreeImage_GetHeight(abitmap),
				0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, // FreeImage uses BGR
				(void*)FreeImage_GetBits(abitmap));
				*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	}
	else
	{
		// Using mipmapping:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, FreeImage_GetWidth(abitmap), FreeImage_GetHeight(abitmap), GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(abitmap));
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256, 256, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture);
	}
	
}
