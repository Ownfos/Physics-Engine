#include "Distance.h"

Distance::Distance(const Distance& d) {
	projection = d.projection;
	relativePosition = d.relativePosition;
	distance = d.distance;
}

Distance::Distance(Vector v1, Vector v2, Vector p) {
	double lengthSquare = (v1 - v2).SquareMagnitude();
	double t = fmax(0, fmin(1, (p - v1)*(v2 - v1) / lengthSquare));//clamp t [0,1] to make sure it's inside the line segment
	projection = v1 + t * (v2 - v1);
	relativePosition = (p - projection);
	distance = relativePosition.Magnitude();
}