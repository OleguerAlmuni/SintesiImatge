#include "World.h"

bool World::readScene( const char *filename )
{
	Reader r;
	
	return r.ReadSceneDescription( filename , scene , cam );
}

Camera World::getCamera( void )
{
	return cam;
}

Scene World::getScene( void )
{
	return scene;
}