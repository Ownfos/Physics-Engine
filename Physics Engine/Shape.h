#pragma once
#pragma comment(lib,"GL/glut32.lib")
#include "GL/glut.h"
#include "Vector.h"

enum TYPE {
	CIRCLE,
	POLYGON
};

class Shape {
public:
	double area;
	virtual TYPE getType() = 0;
	virtual bool isInside(Vector) = 0;
	virtual void draw() = 0;
};