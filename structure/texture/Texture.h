#pragma once

#include "../../libapi.h"
#include "../object/Object.h"

namespace tunage {
	class LIB_API Texture : public Object {
	public:
		~Texture();
		Texture() : Object{}{}
		Texture(std::string name) : Object{ name }{}

		//	Rendering methods
		void render() override;

		//	Load texture methods
		void loadFromFile(std::string path);
		void loadTexture(void* bitmap);

		//	Settings methods
		void setAnisotropic(bool anisotropic);
		void setMipmap(bool useMipmaps);
		void setAnisotropicLevel(int anisotropicLevel);
	private:
		void init();

		//	Default settings
		bool initialized = false;
		unsigned int texId = 0;
		bool useMipmaps = false;
		bool isAnisotropicSupported = false;
		bool anisotropic = false;
		int anisotropicLevel = 1;
		void* bitmap = NULL;
		int bmp_w = 0;
		int bmp_h = 0;
	};
}