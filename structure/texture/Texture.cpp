#include "Texture.h"

#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF

// FreeGLUT:
#include <GL/freeglut.h>
#include <FreeImage.h>
#include <zconf.h>
#include <iostream>
#include <regex>


tunage::Texture::~Texture() {
	glDeleteTextures(1, &texId);
}

void tunage::Texture::render() {
	if (!initialized) {
		initialized = true;
		init();
	} else {
		// Update texture content:
		glBindTexture(GL_TEXTURE_2D, texId);
	}


}

void tunage::Texture::loadFromFile(std::string path) {
	char dir[FILENAME_MAX];
	getcwd(dir, FILENAME_MAX);
	std::cout << "Current dir: " << dir << std::endl;

	std::regex jpg(".*\.(jpg|jpeg)", std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::regex bmp(".*\.bmp", std::regex_constants::ECMAScript | std::regex_constants::icase);

	FREE_IMAGE_FORMAT format = FIF_BMP;

	if(std::regex_search(path, jpg)){
		format = FIF_JPEG;
	} else if (std::regex_search(path, bmp)){
		format = FIF_BMP;
	}

	this->bitmap = FreeImage_Load(format, path.c_str(), 0);

}

void tunage::Texture::loadTexture(unsigned char* texture) {
	this->texture = texture;
}

void tunage::Texture::setAnisotropic(bool anisotropic) {
	this->anisotropic = anisotropic;
}

void tunage::Texture::setMipmap(bool useMipmaps) {
	this->useMipmaps = useMipmaps;
}

void tunage::Texture::setAnisotropicLevel(int anisotropicLevel) {
	this->anisotropicLevel = anisotropicLevel;
}

void tunage::Texture::init() {
	if (strstr((const char *) glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic")) {
		isAnisotropicSupported = true;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicLevel);
	} else {
		isAnisotropicSupported = false;
	}

	glGenTextures(1, &texId);
	texture = new unsigned char[256 * 256 * 4];
	
	for (int i = 0; i < 256*256*4; i++) {
		texture[i] = rand() % 255;
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

	if (!useMipmaps) {
		// Without mipmapping:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap),
				0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, // FreeImage uses BGR
				(void*)FreeImage_GetBits(bitmap));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));
	} else {
		// Using mipmapping:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256, 256, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap->data);
	}

}
