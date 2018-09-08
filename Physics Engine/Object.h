#pragma once
#include "Circle.h"
#include "Polygon.h"

static bool damping = false;

class Object {
public:
	Shape* shape;
	Vector dragPosition;//relative coordinate
	Vector position;
	Vector linearVelocity;
	Vector linearForce;
	Vector impulseAccumulator;
	double angle = 0;//in radian
	double angularVelocity = 0;
	double angularForce = 0;
	double mass = 1;
	double inverseMass = 1;
	double inertia = 1;
	double inverseInertia = 1;
	double restitution = 0.5;
	double staticFriction = 0.6;
	double dynamicFriction = 0.4;
	double linearDamping = 0.005;
	double angularDamping = 0.002;
	double density;
	double r, g, b;

	Object();
	Object(Shape* shape);
	~Object();

	void randomColor();
	void setColor(double r, double g, double b);
	void set();
	void setMass(double mass);
	void setInertia(double inertia);
	//absolute coordinate point
	bool isInside(Vector point);
	//relative coordinate point
	void applyImpulse(Vector impulse, Vector point, double dt);
	void update(double dt);
	void draw();
};