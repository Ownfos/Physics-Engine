#pragma once

#include "Vector.h"

class Distance {
public:
	Vector projection;//closest point
	Vector relativePosition;//from projection to p
	double distance;

	Distance(const Distance& d);
	Distance(Vector v1, Vector v2, Vector p);
};