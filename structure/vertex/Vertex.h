//
// Created by dvitali on 17.11.18.
//

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tunage {
    class Vertex {
    public:
//		Vertex() {};
        Vertex(float posx = 0, float posy = 0, float posz = 0,
			float normalx = 0, float normaly = 0, float normalz = 0,
			float u = 0, float v = 0) : 
			pos{ glm::vec3(posx,posy,posz) }, 
			normal{ glm::vec3(normalx,normaly,normalz) }, 
			uv{ glm::vec2(u,v) } {};
//		Vertex(float posx, float posy, float posz, float normalx, float normaly, float normalz) : pos{ glm::vec3(posx,posy,posz) }, normal{ glm::vec3(normalx,normaly,normalz) } {};
//		Vertex(float posx, float posy, float posz) : pos{ glm::vec3(posx,posy,posz) }{};

		glm::vec3 getPos() const;
		glm::vec3 getNorm() const;
		glm::vec2 getUV() const;

	private:
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
    };
}

