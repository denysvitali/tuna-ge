#pragma once

#include "../node/Node.h"
#include "../material/Material.h"
#include "../vertex/Vertex.h"

namespace tunage {
	class Mesh : public Node {
	public:
		Mesh(std::string name) : Node{name} {}

		void render() override;

		void addVertex(Vertex vertex);

		void setMaterial(Material& material);

		Material getMaterial() const;

	private:
		Material material;
		std::vector<Vertex> faces;
	};
}