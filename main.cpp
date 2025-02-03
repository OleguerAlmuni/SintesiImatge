#include <iostream>
#include "Parser.h"
#include "Delaunay.h"

void main(int argc, const char* argv[])
{
	Mesh mesh;
	std::cout << "Parsing input file. Please wait...\n";

	if (!parsePoints("input1.txt", mesh.vertices))
	{
		printf("ERROR! Could not parse input file\n");
		system("pause");
		return;
	}

	std::cout << " [Done]\n" << std::endl;
	std::cout << "Creating mesh. Please wait...\n";

	triangulateMesh(mesh);

	std::cout << " [Done]\n" << std::endl;
	std::cout << "Saving PLY file. Please wait...\n";

	if (!saveMeshToPLY(mesh, "output1.ply"))
	{
		printf("ERROR! Could not save mesh to PLY file\n");
		system("pause");
		return;
	}

	std::cout << " [Done]\n" << std::endl;
	system("pause");
}

