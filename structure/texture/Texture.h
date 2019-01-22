#pragma once

#include "../../libapi.h"
#include "../object/Object.h"

namespace tunage {
	class LIB_API Texture : public Object {
	public:
		~Texture();
		Texture() : Object{}{}

		explicit Texture(const char* name) : Object{ name }{}

		//	Load texture methods
		void loadFromFile(const char* path);
		void loadTexture(void* bitmap);

		//	Settings methods
		void setAnisotropic(bool anisotropic);
		void setMipmap(bool useMipmaps);
		void setAnisotropicLevel(int anisotropicLevel);
	private:
		void init();

		friend class Material;

		//	Rendering methods
		void render() override;

		//	Default settings
		bool initialized = false;
		unsigned int texId = 0;
		bool useMipmaps = false;
		bool isAnisotropicSupported = false;
		bool anisotropic = false;
		int anisotropicLevel = 1;
		void* bitmap = nullptr;
		int bmp_w = 0;
		int bmp_h = 0;
	};
}