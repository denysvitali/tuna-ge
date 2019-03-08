#pragma once
#include "../../libapi.h"
#include "../node/Node.h"
#include "../material/Material.h"
#include "../vertex/Vertex.h"


namespace tunage {
	class LIB_API Mesh : public Node {
	public:
		Mesh() : Node{} {}
		~Mesh();

		explicit Mesh(const char* name) : Node{name} {}

		//	Rendering methods
		void render() override;

		void render(glm::mat4 pos, Material* mat) override;

		void addVertex(Vertex& vertex);

		void addVertexes(float* positionArr, float* texcoordArr, float* normalArr, unsigned int* facesArr, int numVertices, int numFaces);

		void setMaterial(Material* material);

		Material* getMaterial() const;

		bool isTransparent();

	private:
		void init();
		unsigned int vboVer;
		unsigned int vboInd;
		bool initialized = false;
		float* positionArr;
		float* texcoordArr;
		float* normalArr;
		unsigned int* facesArr;
		int numVertices;
		int numFaces;
		Material* material = nullptr;
		std::vector<Vertex> faces;	//	Vertexes of the mesh object
	};
}