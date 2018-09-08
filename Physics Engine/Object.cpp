#include "Object.h"

Object::Object() {
	randomColor();
}

Object::Object(Shape* shape) : shape(shape) {
	randomColor();
}

Object::~Object() {
	delete shape;
}

void Object::randomColor() {
	r = (double)rand() / RAND_MAX;
	g = (double)rand() / RAND_MAX;
	b = (double)rand() / RAND_MAX;
}

void Object::setColor(double r, double g, double b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

void Object::set() {
	setMass(shape->area);
	//setInertia(shape->area*shape->area);
	///*
	switch (shape->getType()) {
	case CIRCLE:
		setInertia(shape->area*shape->area / PI / 2);
		break;
	case POLYGON:
		setInertia(shape->area*shape->area);
		break;
	}
	//*/
}

void Object::setMass(double mass) {
	this->mass = mass;
	if (mass == 0)
		inverseMass = 0;
	else
		inverseMass = 1 / mass;
}

void Object::setInertia(double inertia) {
	this->inertia = inertia;
	if (inertia == 0)
		inverseInertia = 0;
	else
		inverseInertia = 1 / inertia;
}

//absolute coordinate point
bool Object::isInside(Vector point) {
	return shape->isInside((point - position).Rotated(-angle));
}

//relative coordinate point
void Object::applyImpulse(Vector impulse, Vector point, double dt) {
	linearForce += impulse / dt * 1000;
	angularForce += (point ^ impulse).z / dt * 1000;
}

void Object::update(double dt) {
	dt /= 1000;
	//integration
	linearVelocity += linearForce * inverseMass*dt;
	position += linearVelocity * dt;
	angularVelocity += angularForce * inverseInertia*dt;
	angle += angularVelocity * dt;
	//damping
	if (damping) {
		double l = fmin(linearDamping, linearVelocity.Magnitude());
		double a = fmin(angularDamping, abs(angularVelocity));
		Vector vl = linearVelocity;
		vl.Normalize();
		linearVelocity -= vl * l;
		if (angularVelocity != 0)
			angularVelocity -= angularVelocity / abs(angularVelocity)*a;
	}
	//
	linearForce = Vector();
	angularForce = 0;
}

void Object::draw() {
	glTranslated(position.x, position.y, 0);
	glRotated(degree(angle), 0, 0, 1);
	glColor3d(r, g, b);
	if (mass == 0 && inertia == 0)
		glColor3d(1, 0, 0);
	shape->draw();
	glLoadIdentity();
}