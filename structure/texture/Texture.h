#pragma once

#include <FreeImage.h>
#include "../object/Object.h"

namespace tunage {
	class Texture : public Object {
	public:
		~Texture();
		Texture(std::string name) : Object{ name }{}
		void render() override;
		void loadFromFile(std::string path);
		void loadTexture(unsigned char *texture);
		void setAnisotropic(bool anisotropic);
		void setMipmap(bool useMipmaps);
		void setAnisotropicLevel(int anisotropicLevel);

	private:

		void init();

		bool initialized = false;
		unsigned char *texture;
		FIBITMAP* bitmap;
		unsigned int texId = 0;
		bool useMipmaps = false;
		bool isAnisotropicSupported = false;
		bool anisotropic = false;
		int anisotropicLevel = 1;
	};
}