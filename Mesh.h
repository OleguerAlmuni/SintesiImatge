#pragma once

#include <vector>

struct Vertex
{
	float x;
	float y;
	float z;
};

struct Face
{
	unsigned int v1;
	unsigned int v2;
	unsigned int v3;
};

struct Mesh
{
	std::vector< Vertex > vertices;
	std::vector< Face > faces;
};

bool saveMeshToPLY(const Mesh &mesh, const char* path);