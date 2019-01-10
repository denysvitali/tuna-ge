#pragma once

class OvMesh {
public:
	enum class Subtype : int ///< Kind of mesh
	{
		// Foundation types:
				DEFAULT = 0,
		NORMALMAPPED,
		TESSELLATED,

		// Terminator:
				LAST,
	};
};
