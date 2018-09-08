#include "Polygon.h"

Polygon::Polygon(std::initializer_list<Vector> list) {
	vertices = list;
	for (int i = 0; i < vertices.size(); i++) {
		int j = (i + 1) % vertices.size();

		Vector edge = vertices[j] - vertices[i];
		edges.push_back(edge);

		Vector normal = edge.Perpendicular(true);
		normal.Normalize();
		normals.push_back(normal);
	}
	set();
}

Polygon::Polygon(std::vector<Vector> list) {
	vertices = list;
	for (int i = 0; i < vertices.size(); i++) {
		int j = (i + 1) % vertices.size();

		Vector edge = vertices[j] - vertices[i];
		edges.push_back(edge);

		Vector normal = edge.Perpendicular(true);
		normal.Normalize();
		normals.push_back(normal);
	}
	set();
}

void Polygon::set() {
	//caculate area
	int count = vertices.size();
	area = 0;
	for (int i = 0; i < count; i++) {
		area += (vertices[i] ^ vertices[(i + 1) % count]).z;
	}
	area /= 2;
	//find center of mass
	Vector center;
	for (int i = 0; i < count; i++) {
		double c = (vertices[i] ^ vertices[(i + 1) % count]).z;
		center += (vertices[i] + vertices[(i + 1) % count])*c;
	}
	center /= area * 6;
	//adjust position
	for (int i = 0; i < count; i++) {
		vertices[i] -= center;
	}
}

bool Polygon::isConvex() {
	int size = vertices.size();
	for (int i = 0; i < size; i++) {
		if ((edges[i] ^ edges[(i + 1) % size]).z < 0)
			return false;
	}
	return true;
}

TYPE Polygon::getType() {
	return POLYGON;
}

bool Polygon::isInside(Vector point) {
	for (int i = 0; i < edges.size(); i++) {
		if ((edges[i] ^ (point - vertices[i])).z < 0)
			return false;
	}
	return true;
}

void Polygon::draw() {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < vertices.size(); i++) {
		glVertex2d(vertices[i].x, vertices[i].y);
	}
	glEnd();
	glBegin(GL_POINTS);
	glVertex2d(0, 0);
	glEnd();
}