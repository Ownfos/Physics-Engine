#pragma once
#include "Shape.h"
#include "Radian.h"
#include "Vector.h"
#include <vector>

class Polygon :public Shape {
public:
	std::vector<Vector> vertices;
	std::vector<Vector> edges;
	std::vector<Vector> normals;

	Polygon(std::initializer_list<Vector> list);
	Polygon(std::vector<Vector> list);

	void set();
	bool isConvex();
	virtual TYPE getType() override;
	virtual bool isInside(Vector point) override;
	virtual void draw() override;
};