#pragma once
#include "Shape.h"
#include "Radian.h"

#define SEGMENT 20

class Circle :public Shape {
public:
	double radius;

	Circle(double radius);

	virtual TYPE getType() override;
	virtual bool isInside(Vector point) override;
	virtual void draw() override;
};