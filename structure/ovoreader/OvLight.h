#pragma once

class OvLight {
public:
	enum class Subtype : int ///< Kind of light
	{
		// Foundation types:
				OMNI = 0,
		DIRECTIONAL,
		SPOT,

		// Terminator:
				LAST,
	};
};