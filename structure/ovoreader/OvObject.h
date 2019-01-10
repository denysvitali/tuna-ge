#pragma once


class OvObject {
public:
	enum class Type : int  ///< Type of entities
	{
		// Foundation types:
				OBJECT = 0,
		NODE,
		OBJECT2D,
		OBJECT3D,
		LIST,

		// Derived classes:
				BUFFER,
		SHADER,
		TEXTURE,
		FILTER,
		MATERIAL,
		FBO,
		QUAD,
		BOX,
		SKYBOX,
		FONT,
		CAMERA,
		LIGHT,
		BONE,
		MESH,       // Keep them...
		SKINNED, // ...consecutive
		INSTANCED,
		PIPELINE,
		EMITTER,

		// Animation type
				ANIM,

		// Physics related:
				PHYSICS,

		// Terminator:
				LAST,
	};
};
