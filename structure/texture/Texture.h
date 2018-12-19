#pragma once
#include "../object/Object.h"

namespace tunage {
	class Texture : public Object {
	public:

		~Texture();
		Texture(std::string name) : Object{ name }{}
		void render() override;
		void loadTextureFromFile();
		void loadTexture(unsigned char *texture);
		void setAnisotropic(bool anisotropic);
		void setMipmap(bool useMipmaps);
		void setAnisotropicLevel(int anisotropicLevel);

	private:

		void init();

		bool initialized = false;
		unsigned char *texture;
		unsigned int texId = 0;
		bool useMipmaps = false;
		bool isAnisotropicSupported = false;
		bool anisotropic = false;
		int anisotropicLevel = 1;
	};
}