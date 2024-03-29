#pragma once

#include "../../libapi.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tunage {
	//	Class used to store a vertex data
    class LIB_API Vertex {
    public:
        Vertex(float posx = 0, float posy = 0, float posz = 0,
			float normalx = 0, float normaly = 0, float normalz = 0,
			float u = 0, float v = 0) : 
			pos{ glm::vec3(posx,posy,posz) }, 
			normal{ glm::vec3(normalx,normaly,normalz) }, 
			uv{ glm::vec2(u,v) } {};

		glm::vec3 getPos() const;
		glm::vec3 getNorm() const;
		glm::vec2 getUV() const;

	private:
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
    };
}

