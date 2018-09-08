#include "Circle.h"

Circle::Circle(double radius) :radius(radius) {
	area = radius * radius*PI;
}

TYPE Circle::getType() {
	return CIRCLE;
}

bool Circle::isInside(Vector point) {
	if (point.Magnitude() < radius)
		return true;
	else
		return false;
}

void Circle::draw() {
	glBegin(GL_LINES);
	glVertex2d(0, 0);
	glVertex2d(radius, 0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < SEGMENT; i++) {
		double angle = 2 * PI * i / SEGMENT;
		glVertex2d(
			radius*cos(angle),
			radius*sin(angle)
		);
	}
	glEnd();
}