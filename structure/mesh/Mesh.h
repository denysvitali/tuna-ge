#pragma once
#include "../../libapi.h"
#include "../node/Node.h"
#include "../material/Material.h"
#include "../vertex/Vertex.h"


namespace tunage {
	class LIB_API Mesh : public Node {
	public:
		Mesh() : Node{} {}

		explicit Mesh(const char* name) : Node{name} {}

		void render() override;

		void render(glm::mat4 pos, Material* mat) override;

		void addVertex(Vertex vertex);

		void setMaterial(Material* material);

		Material* getMaterial() const;

		bool isTransparent();

	private:
		Material* material = nullptr;
		std::vector<Vertex> faces;
	};
}