#pragma once
#include "Object.h"
#include "Projection.h"
#include "Distance.h"
#include <vector>

static bool perfectCorrection = false;

class Collision {
public:
	Object* a;
	Object* b;
	Vector normal;
	//absolute coordinate
	//used for drawing collision points
	std::vector<Vector> contacts;
	double penetration;
	double restitution;
	double staticFriction;
	double dynamicFriction;

	Collision(Object* a, Object* b);

	//collision types
	void CircletoCircle();
	void CircletoPolygon();
	void PolygontoCircle();
	void PolygontoPolygon();

	//find collision normal and collision points
	void solve();
	//initialize
	void initializeCollision();
	//apply force to objects
	void applyImpulse(double dt);
	void positionalCorrection();
};