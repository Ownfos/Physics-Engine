#include "Projection.h"

Projection::Projection(const Projection& p) {
	min = p.min;
	max = p.max;
	minIndex = p.minIndex;
	maxIndex = p.maxIndex;
}

Projection::Projection(Vector axis, Polygon* p, Vector position, double angle) {
	min = MAX;
	max = -MAX;

	for (int i = 0; i < p->vertices.size(); i++) {
		double d = axis * (p->vertices[i].Rotated(angle));
		if (d < min) {
			min = d;
			minIndex = i;
		}
		if (d > max) {
			max = d;
			maxIndex = i;
		}
	}
	min += axis * position;
	max += axis * position;
}