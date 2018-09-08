#include "Spring.h"

Spring::Spring(const Spring& s) {
	o1 = s.o1;
	o2 = s.o2;
	p1 = s.p1;
	p2 = s.p2;
	distance = s.distance;
}

Spring::Spring(Object* o1, Object* o2, Vector p1, Vector p2) {
	this->o1 = o1;
	this->o2 = o2;
	this->p1 = p1;
	this->p2 = p2;
	distance = ((o1->position + p1.Rotated(o1->angle)) - (o2->position + p2.Rotated(o2->angle))).Magnitude();
}

void Spring::solve(double dt) {
	v1 = o1->position + p1.Rotated(o1->angle);
	v2 = o2->position + p2.Rotated(o2->angle);
	Vector rp = v2 - v1;
	double delta = rp.Magnitude() - distance;
	rp.Normalize();
	impulse = rp * delta*coefficient;
	if (isJoint)
		impulse /= coefficient * dt * 2 / 1000;
	o1->applyImpulse(impulse, v1 - o1->position, dt);
	o2->applyImpulse(-impulse, v2 - o2->position, dt);
}