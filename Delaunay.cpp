#include <iostream>
#include <list>
#include "Delaunay.h"

// Nombres y apellidos: ___________

//this function is called from main, receives a Mesh object
//which containss a std::vector of vertices and another of faces
void triangulateMesh(Mesh& mesh) {

	superTriangle(mesh);
	for (int i = 0; i < mesh.vertices.size() - 3; i++) { // El -3 viene de los vertices creados por superTriangle
		std::vector<Edge> edges;

		for (int j = 0; j < mesh.faces.size(); j++) {
			if (isInsideCircumCircle(mesh.vertices[mesh.faces[j].v1], mesh.vertices[mesh.faces[j].v2], mesh.vertices[mesh.faces[j].v3], mesh.vertices[i])) {
				Edge edge1;

				edge1.v1 = mesh.faces[j].v1;
				edge1.v2 = mesh.faces[j].v2;
				edges.push_back(edge1);

				Edge edge2;
				edge2.v1 = mesh.faces[j].v2;
				edge2.v2 = mesh.faces[j].v3;
				edges.push_back(edge2);

				Edge edge3;
				edge3.v1 = mesh.faces[j].v3;
				edge3.v2 = mesh.faces[j].v1;
				edges.push_back(edge3);

				mesh.faces.erase(mesh.faces.begin() + j);
				j--;

				printf("He borrado una cara");
			}
		}

		removeDuplicate(edges);

		Face face;
		face.v3 = i;
		for (int k = 0; k < edges.size(); k++) {
			face.v1 = edges[k].v1;
			face.v2 = edges[k].v2;
			mesh.faces.push_back(face);
			printf("He añadido una cara");
		}

		edges.clear();
	}

	removeTriangle(mesh);
}


void removeDuplicate(std::vector<Edge>& e) {
	/*for (int i = 0; i < e.size(); i++) {
		for (int j = 0; j < e.size(); j++) {
			if (e[i].v1 == e[j].v1 && e[i].v2 == e[j].v2 && i != j) {
				e.erase(e.begin() + j);
				e.erase(e.begin() + i);
			}
		}
	}*/

	std::vector<Edge> ed;
	bool same = false;
	for (int i = 0; i < e.size(); i++) {
		same = false;
		for (int j = 0; j < e.size(); j++) {
			if ((e[i].v1 == e[j].v1 && e[i].v2 == e[j].v2) || (e[i].v1 == e[j].v2 && e[i].v2 == e[j].v1) && i != j) {
				same = true;
				printf("He encontrado un repetido");
			}
		}
		if (same == false) {
			ed.push_back(e[i]);
		}
	}
	e = ed;
}

//create temporary triangle that contains all vertices
//(adds three temporary vertices to mesh)
void superTriangle(Mesh& mesh) {

	double
		xmin = mesh.vertices[0].x,
		xmax = xmin,
		ymin = mesh.vertices[0].y,
		ymax = ymin;

	for (int i = 1; i < mesh.vertices.size(); i++) {
		if (mesh.vertices[i].x < xmin) xmin = mesh.vertices[i].x;
		if (mesh.vertices[i].x > xmax) xmax = mesh.vertices[i].x;
		if (mesh.vertices[i].y < ymin) ymin = mesh.vertices[i].y;
		if (mesh.vertices[i].y > ymax) ymax = mesh.vertices[i].y;
	}

	double
		dx = xmax - xmin,
		dy = ymax - ymin,
		dmax = (dx > dy) ? dx : dy,
		xmid = (xmax + xmin) / 2.0,
		ymid = (ymax + ymin) / 2.0;

	Vertex
		v1{ xmid - 20 * dmax, ymid - dmax, 0.0 },
		v2{ xmid + 20 * dmax, ymid - dmax, 0.0 },
		v3{ xmid, ymid + 20 * dmax, 0.0 };

	mesh.vertices.push_back(v1);
	mesh.vertices.push_back(v2);
	mesh.vertices.push_back(v3);
	mesh.faces.push_back(Face{ mesh.vertices.size() - 3, mesh.vertices.size() - 2, mesh.vertices.size() - 1 });

}



//remove any faces that were connected to the temporary super triangle
void removeTriangle(Mesh& mesh) {

	for (int i = 0; i < mesh.faces.size(); i++){
		if ((mesh.faces[i].v1 == mesh.vertices.size() - 3) || (mesh.faces[i].v1 == mesh.vertices.size() - 2) || (mesh.faces[i].v1 == mesh.vertices.size() - 1) ||
			(mesh.faces[i].v2 == mesh.vertices.size() - 3) || (mesh.faces[i].v2 == mesh.vertices.size() - 2) || (mesh.faces[i].v2 == mesh.vertices.size() - 1) ||
			(mesh.faces[i].v3 == mesh.vertices.size() - 3) || (mesh.faces[i].v3 == mesh.vertices.size() - 2) || (mesh.faces[i].v3 == mesh.vertices.size() - 1))
		{
			mesh.faces.erase(mesh.faces.begin() + i);
			i = i - 1;
		}
	}

	int size = mesh.vertices.size();
	mesh.vertices.erase(mesh.vertices.begin() + (size - 1));
	mesh.vertices.erase(mesh.vertices.begin() + (size - 2));
	mesh.vertices.erase(mesh.vertices.begin() + (size - 3));


}


//returns true if Vertex 'point' is within the circumcircle of triangle ABV
bool isInsideCircumCircle(const Vertex A, const Vertex B, const Vertex C, const Vertex &point){
	double m1, m2, mx1, mx2, my1, my2, xc, yc, r;
	double dx, dy, rsqr, drsqr;

	if (abs(A.y - B.y) < EPSILON && abs(B.y - C.y) < EPSILON)
		return(false);
	if (abs(B.y - A.y) < EPSILON){
		m2 = -(C.x - B.x) / (C.y - B.y);
		mx2 = (B.x + C.x) / 2.0;
		my2 = (B.y + C.y) / 2.0;
		xc = (B.x + A.x) / 2.0;
		yc = m2 * (xc - mx2) + my2;
	}
	else if (abs(C.y - B.y) < EPSILON){
		m1 = -(B.x - A.x) / (B.y - A.y);
		mx1 = (A.x + B.x) / 2.0;
		my1 = (A.y + B.y) / 2.0;
		xc = (C.x + B.x) / 2.0;
		yc = m1 * (xc - mx1) + my1;
	}
	else{
		m1 = -(B.x - A.x) / (B.y - A.y);
		m2 = -(C.x - B.x) / (C.y - B.y);
		mx1 = (A.x + B.x) / 2.0;
		mx2 = (B.x + C.x) / 2.0;
		my1 = (A.y + B.y) / 2.0;
		my2 = (B.y + C.y) / 2.0;
		xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
		yc = m1 * (xc - mx1) + my1;
	}
	dx = B.x - xc;
	dy = B.y - yc;
	rsqr = dx * dx + dy * dy;
	dx = point.x - xc;
	dy = point.y - yc;
	drsqr = dx * dx + dy * dy;
	return((drsqr <= rsqr) ? true : false);

}






