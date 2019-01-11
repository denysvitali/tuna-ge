#include "Texture.h"

#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF

// FreeGLUT:
#include <GL/freeglut.h>
#include <FreeImage.h>
#include <iostream>
#include <regex>
#include <stdio.h>

#ifdef _WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


tunage::Texture::~Texture() {
	glDeleteTextures(1, &texId);
	std::cout << "distrutto id: " << getId() << std::endl;
}

void tunage::Texture::render() {
	glEnable(GL_TEXTURE_2D);
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
	GetCurrentDir(dir, FILENAME_MAX);
	std::cout << "Current dir: " << dir << std::endl;

	std::regex jpg(".*\.(jpg|jpeg)", std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::regex bmp(".*\.bmp", std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::regex png(".*\.png", std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::regex dds(".*\.dds", std::regex_constants::ECMAScript | std::regex_constants::icase);

	FREE_IMAGE_FORMAT format = FIF_BMP;

	if(std::regex_search(path, jpg)){
		format = FIF_JPEG;
	} else if (std::regex_search(path, bmp)){
		format = FIF_BMP;
	} else if (std::regex_search(path, png)){
		format = FIF_PNG;
	} else if (std::regex_search(path, dds)){
		format = FIF_DDS;
	}

	FIBITMAP* bitmap = FreeImage_Load(format, path.c_str(), 0);
	this->bmp_h = FreeImage_GetHeight(bitmap);
	this->bmp_w = FreeImage_GetWidth(bitmap);


	this->bitmap = FreeImage_GetBits(FreeImage_ConvertTo32Bits(bitmap));


}

void tunage::Texture::loadTexture(void* bitmap) {
	this->bitmap = bitmap;
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

	FIBITMAP* bitmap;

	if (!useMipmaps) {
		// Without mipmapping:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				this->bmp_w, this->bmp_h,
				0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, // FreeImage uses BGR
				this->bitmap);
	} else {
		// Using mipmapping:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, this->bmp_w, this->bmp_h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, this->bitmap);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256, 256, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap->data);
	}

}
