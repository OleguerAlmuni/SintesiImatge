#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"
	// Surface material for shading.
	class Material 
	{
		public:
			Color diffuse;      // Diffuse color.
			Color specular;     // Color of highlights.
			Color emission;     // Emitted light.
			int   type;         // Reserved for future use.
			float phong_exp;    // Phong exponent for specular highlights.
			float reflectivity; // Weight given to mirror reflection, between 0 and 1.
			float refractive_index;	// (vel. llum en el buit) / (vel. llum en aquest material)
			float opacity;			// [0-1] 0:transparent, 1:opac
			bool  isEmitter() const { return ( emission.red != 0 || emission.blue != 0 || emission.green != 0 ); } 
	};

#endif