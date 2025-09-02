#ifndef APPMAIN_H
#define APPMAIN_H


#include "World.h"
#include "Raytracer.h"

#include <GL/glut.h>

#define RESOLUTIONX 600
#define RESOLUTIONY 600

Raytracer g_raytracer( RESOLUTIONX , RESOLUTIONY );
World w;

#endif