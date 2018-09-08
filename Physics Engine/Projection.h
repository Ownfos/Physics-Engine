#pragma once

#include "Vector.h"
#include "Polygon.h"

#define MAX 99999999

class Projection {
public:
	double min;
	double max;
	int minIndex;
	int maxIndex;

	Projection(const Projection& p);
	Projection(Vector axis, Polygon* p, Vector position, double angle);
};