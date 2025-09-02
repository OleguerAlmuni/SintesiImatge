#ifndef SCENE_H
#define SCENE_H

#include "PointLight.h"
#include "Object.h"

class Scene 
{
	public:
		
		Color ambient;        // The single ambient light.
		Color bgcolor;        // Background color, if ray does not hit anything. 
		
		Object *first;        // The first of a list of objects.
};

#endif