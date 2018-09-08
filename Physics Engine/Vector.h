#pragma once
#include<math.h>

#define TOL 0.0001

class Vector
{
public:
	double x;
	double y;
	double z;

	Vector();
	Vector(double xi, double yi, double zi = 0);

	double Magnitude();
	double SquareMagnitude();
	void Normalize();
	void Reverse();
	Vector Rotated(double);
	Vector Perpendicular(bool);

	Vector& operator=(Vector u);
	Vector& operator+=(Vector u);
	Vector& operator-=(Vector u);
	Vector& operator*=(double s);
	Vector& operator/=(double s);
	Vector operator+(Vector u);
	Vector operator-(Vector u);
	Vector operator*(double s);
	Vector operator/(double s);
	Vector operator-();
	Vector operator^(Vector u);
	double operator*(Vector u);

	friend Vector operator*(double s, Vector u);
};

