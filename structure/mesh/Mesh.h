#pragma once
#include "../../libapi.h"
#include "../node/Node.h"
#include "../material/Material.h"
#include "../vertex/Vertex.h"


namespace tunage {
	class LIB_API Mesh : public Node {
	public:
		Mesh();
		~Mesh();

		explicit Mesh(const char* name) : Node{name} {}

		//	Rendering methods
		void render() override;

		void render(glm::mat4 pos, Material* mat) override;

		void init(float* positionArr, float* texcoordArr, float* normalArr, unsigned int* facesArr, int numVertices, int numFaces);

		void setMaterial(Material* material);

		Material* getMaterial() const;

		bool isTransparent();

	private:
		unsigned int vaoId;
		unsigned int vboId[2];
		unsigned int vertVboId;
		unsigned int faceVboId;
		int numVertices;
		int numFaces;
		bool initialized = false;
		Material* material = nullptr;
	};
}