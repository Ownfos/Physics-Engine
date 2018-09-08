#include "Collision.h"

Collision::Collision(Object* a, Object* b) :a(a), b(b) {

}

void Collision::solve() {
	if (a->inverseMass == 0 && b->inverseMass == 0)
		return;
	if (a->shape->getType() == CIRCLE && b->shape->getType() == CIRCLE)
		CircletoCircle();
	if (a->shape->getType() == CIRCLE && b->shape->getType() == POLYGON)
		CircletoPolygon();
	if (a->shape->getType() == POLYGON && b->shape->getType() == CIRCLE)
		PolygontoCircle();
	if (a->shape->getType() == POLYGON && b->shape->getType() == POLYGON)
		PolygontoPolygon();
}

void Collision::CircletoCircle() {
	Circle* A = (Circle*)a->shape;
	Circle* B = (Circle*)b->shape;

	normal = b->position - a->position;
	double distance = normal.SquareMagnitude();
	double radius = A->radius + B->radius;
	if (distance >= radius * radius) {
		return;
	}

	distance = sqrt(distance);
	if (distance > TOL)//if not on the same position(distance==0)
		normal /= distance;//normalize
	else
		normal = Vector(1, 0);
	penetration = radius - distance;
	contacts.push_back(normal*A->radius + a->position);
}

void Collision::CircletoPolygon() {
	Circle* A = (Circle*)a->shape;
	Polygon* B = (Polygon*)b->shape;
	///*my method
	bool inside = b->isInside(a->position);
	double min = MAX;
	int size = B->vertices.size();
	for (int i = 0; i < size; i++) {
		Distance d = Distance(b->position + B->vertices[i].Rotated(b->angle), b->position + (B->vertices[i] + B->edges[i]).Rotated(b->angle), a->position);
		Vector n = B->normals[i].Rotated(b->angle);
		double p = d.relativePosition*n - A->radius;
		if (p < 0 && -p < min && (d.distance<A->radius || inside)) {
			min = -p;
			contacts.clear();
			contacts.push_back(d.projection);
			normal = d.relativePosition;
			normal.Normalize();
			if (inside) {
				penetration = A->radius + d.distance;
			}
			else {
				penetration = A->radius - d.distance;
				normal.Reverse();
			}
		}
	}
	//*/
}

void Collision::PolygontoCircle() {
	Object* temp = a;
	a = b;
	b = temp;
	CircletoPolygon();
}

void Collision::PolygontoPolygon() {
	Polygon* A = (Polygon*)a->shape;
	Polygon* B = (Polygon*)b->shape;

	//penetration depth
	double overlap = MAX;
	//minimum translation vector
	Vector mtv;
	//index of vertex closest to collision point
	int aIndex;
	int bIndex;

	int size;

	size = A->normals.size();
	for (int i = 0; i < size; i++) {
		Vector axis = A->normals[i].Rotated(a->angle);
		Projection pa = Projection(axis, A, a->position, a->angle);
		Projection pb = Projection(axis, B, b->position, b->angle);
		if (pa.min > pb.max || pa.max < pb.min) {
			return;
		}
		else {
			//true when a penetrates b
			bool s = fabs(pb.max - pa.min) < fabs(pb.min - pa.max);
			double o = s ? fabs(pb.max - pa.min) : fabs(pb.min - pa.max);
			if (o < overlap) {
				overlap = o;
				if (s) {
					mtv = -axis;
					aIndex = pa.minIndex;
					bIndex = pb.maxIndex;
				}
				else {
					mtv = axis;
					aIndex = pa.maxIndex;
					bIndex = pb.minIndex;
				}
			}
		}
	}
	size = B->normals.size();
	for (int i = 0; i < size; i++) {
		Vector axis = B->normals[i].Rotated(b->angle);
		Projection pa = Projection(axis, A, a->position, a->angle);
		Projection pb = Projection(axis, B, b->position, b->angle);
		if (pb.min > pa.max || pb.max < pa.min) {
			return;
		}
		else {
			//true when b penetrates a
			bool s = fabs(pa.max - pb.min) < fabs(pa.min - pb.max);
			double o = s ? fabs(pa.max - pb.min) : fabs(pa.min - pb.max);
			if (o < overlap) {
				overlap = o;
				mtv = s ? axis : -axis;
				if (s) {
					mtv = axis;
					aIndex = pa.maxIndex;
					bIndex = pb.minIndex;
				}
				else {
					mtv = -axis;
					aIndex = pa.minIndex;
					bIndex = pb.maxIndex;
				}
			}
		}
	}
	normal = mtv;
	penetration = overlap;

	//absolute coordinate
	//av[1] is the closest vertex to collision point
	Vector av[3];
	Vector bv[3];
	//the end point of the edge(ref or inc) and edge vector(from a1 to a2)
	Vector a1, a2, ae;
	Vector b1, b2, be;
	for (int i = -1; i <= 1; i++) {
		int size = A->vertices.size();
		int ind = ((aIndex + i) % size + size) % size;
		av[i + 1] = A->vertices[ind].Rotated(a->angle) + a->position;
	}
	for (int i = -1; i <= 1; i++) {
		int size = B->vertices.size();
		int ind = ((bIndex + i) % size + size) % size;
		bv[i + 1] = B->vertices[ind].Rotated(b->angle) + b->position;
	}
	//check which edge is more perpendicular to collision normal
	Vector e1, e2;
	e1 = av[1] - av[0];
	e1.Normalize();
	e2 = av[2] - av[1];
	e2.Normalize();
	if (fabs(e1*normal) < fabs(e2*normal)) {
		a1 = av[0];
		a2 = av[1];
		ae = e1;
	}
	else {
		a1 = av[1];
		a2 = av[2];
		ae = e2;
	}
	e1 = bv[1] - bv[0];
	e1.Normalize();
	e2 = bv[2] - bv[1];
	e2.Normalize();
	if (fabs(e1*normal) < fabs(e2*normal)) {
		b1 = bv[0];
		b2 = bv[1];
		be = e1;
	}
	else {
		b1 = bv[1];
		b2 = bv[2];
		be = e2;
	}
	Vector ref[2];
	Vector inc[2];
	if (fabs(ae*normal) < fabs(be*normal)) {
		ref[0] = a1;
		ref[1] = a2;
		inc[0] = b1;
		inc[1] = b2;
	}
	else {
		ref[0] = b1;
		ref[1] = b2;
		inc[0] = a1;
		inc[1] = a2;
	}
	//clip incident edge
	std::vector<Vector> cp;
	cp.push_back(inc[0]);
	cp.push_back(inc[1]);
	Vector v1, v2, edge;
	double d1, d2;
	//from ref[0] to ref[1]
	v1 = cp[0];
	v2 = cp[1];
	edge = ref[1] - ref[0];
	d1 = edge * (v1 - ref[0]);
	d2 = edge * (v2 - ref[0]);
	cp.clear();
	if (d1 >= 0)
		cp.push_back(v1);
	if (d2 >= 0)
		cp.push_back(v2);
	if (d1*d2 < 0) {
		Vector v = d1 * v2 - d2 * v1;//v1, v2의 d1:d2 내분점(-붙어있어서 외분처럼 보이지만 둘중 하나가 음수이기에 결국 내분으로 변한다)
		v /= d1 - d2;
		cp.push_back(v);
	}
	//from ref[1] to ref[0]
	v1 = cp[0];
	v2 = cp[1];
	edge = ref[0] - ref[1];
	d1 = edge * (v1 - ref[1]);
	d2 = edge * (v2 - ref[1]);
	cp.clear();
	if (d1 >= 0)
		cp.push_back(v1);
	if (d2 >= 0)
		cp.push_back(v2);
	if (d1*d2 < 0) {
		Vector v = d1 * v2 - d2 * v1;
		v /= d1 - d2;
		cp.push_back(v);
	}
	//check whether cp is on the left side of reference edge
	v1 = cp[0];
	v2 = cp[1];
	edge = ref[1] - ref[0];
	d1 = (edge ^ (v1 - ref[0])).z;
	d2 = (edge ^ (v2 - ref[0])).z;
	cp.clear();
	if (d1 >= 0)
		cp.push_back(v1);
	if (d2 >= 0)
		cp.push_back(v2);
	//put collision point(s)
	for (int i = 0; i<cp.size(); i++)
		contacts.push_back(cp[i]);
}

void Collision::initializeCollision() {
	restitution = (a->restitution < b->restitution) ? a->restitution : b->restitution;
	staticFriction = (a->staticFriction + b->staticFriction) / 2;
	dynamicFriction = (a->dynamicFriction + b->dynamicFriction) / 2;
}

void Collision::applyImpulse(double dt) {
	for (int i = 0; i < contacts.size(); i++) {
		Vector ra = contacts[i] - a->position;
		Vector rb = contacts[i] - b->position;

		//Vector rv = b->linearVelocity - a->linearVelocity;
		Vector rv = (b->linearVelocity + (Vector(0, 0, b->angularVelocity) ^ rb)) - (a->linearVelocity + (Vector(0, 0, a->angularVelocity) ^ ra));

		//velocity along contact normal
		double cv = normal * rv;

		//if seperating, don't solve collision
		if (cv > 0)
			return;

		//double j = -(1 + restitution)*cv / (a->inverseMass + b->inverseMass);
		//double j = -(1 + restitution)*cv / (a->inverseMass + b->inverseMass + normal*((ra^normal)^ra)*a->inverseInertia + normal*((rb^normal)^rb)*b->inverseInertia);
		double j = -(1 + restitution)*cv / (a->inverseMass + b->inverseMass + (ra^normal)*(ra^normal)*a->inverseInertia + (rb^normal)*(rb^normal)*b->inverseInertia);
		j /= contacts.size();

		Vector impulse = normal * j;

		//friction
		Vector t = rv - normal * (rv*normal);
		t.Normalize();

		///*
		double jt = -rv * t / (a->inverseMass + b->inverseMass);
		jt /= contacts.size();
		Vector tangentImpulse;
		if (fabs(jt) < j*staticFriction)
			tangentImpulse = t * jt;
		else
			tangentImpulse = t * -j * dynamicFriction;
		//*/
		/*
		Vector tangentImpulse;
		if (rv.Magnitude() == 0)
		tangentImpulse = -t*(t*impulse);
		else
		tangentImpulse = -t*dynamicFriction*j;

		tangentImpulse /= contacts.size();
		//*/
		a->applyImpulse(-impulse, ra, dt);
		b->applyImpulse(impulse, rb, dt);

		a->applyImpulse(-tangentImpulse, ra, dt);
		b->applyImpulse(tangentImpulse, rb, dt);
	}
}

void Collision::positionalCorrection() {
	double allowance, percent;
	if (perfectCorrection) {
		allowance = 0;
		percent = 1;
	}
	else {
		allowance = 0.05;
		percent = 0.4;
	}
	Vector correction = (fmax(penetration - allowance, 0.0) / (a->inverseMass + b->inverseMass))*normal*percent;
	a->position -= correction * a->inverseMass;
	b->position += correction * b->inverseMass;
}