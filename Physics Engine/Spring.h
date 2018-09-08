#pragma once
#include "Object.h"

class Spring {
public:
	Object *o1, *o2;
	Vector p1, p2;//relative coordinate
	Vector impulse;
	Vector v1, v2;//velocity of joined points
	double distance;//initial distance
	double coefficient = 1;
	bool isJoint = true;

	Spring(const Spring& s);
	Spring(Object* o1, Object* o2, Vector p1, Vector p2);

	void solve(double dt);
};