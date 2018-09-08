#include "Vector.h"

Vector::Vector()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(double xi, double yi, double zi)
{
	x = xi;
	y = yi;
	z = zi;
}

double Vector::Magnitude()
{
	return sqrt(x*x + y * y + z * z);
}

double Vector::SquareMagnitude()
{
	return x * x + y * y + z * z;
}

void Vector::Normalize()
{
	double m = sqrt(x*x + y * y + z * z);
	if (m <= TOL) m = 1;
	x /= m;
	y /= m;
	z /= m;
	if (fabs(x) < TOL) x = 0;
	if (fabs(y) < TOL) y = 0;
	if (fabs(z) < TOL) z = 0;
}

void Vector::Reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

//in radian
Vector Vector::Rotated(double angle) {
	return Vector(x*cos(angle) - y * sin(angle), y*cos(angle) + x * sin(angle));
}

Vector Vector::Perpendicular(bool isRight) {
	if (isRight)
		return Vector(y, -x);
	else
		return Vector(-y, x);
}

Vector& Vector::operator=(Vector u) {
	x = u.x;
	y = u.y;
	z = u.z;
	return *this;
}

Vector& Vector::operator+=(Vector u)
{
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

Vector& Vector::operator-=(Vector u)
{
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

Vector Vector::operator+(Vector u)
{
	return Vector(x + u.x, y + u.y, z + u.z);
}

Vector Vector::operator-(Vector u)
{
	return Vector(x - u.x, y - u.y, z - u.z);
}

Vector& Vector::operator*=(double s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector& Vector::operator/=(double s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector Vector::operator*(double s)
{
	return Vector(x*s, y*s, z*s);
}

Vector operator*(double s, Vector u)
{
	return Vector(u.x*s, u.y*s, u.z*s);
}

Vector Vector::operator/(double s)
{
	return Vector(x / s, y / s, z / s);
}

Vector Vector::operator-() {
	return Vector(-x, -y, -z);
}

Vector Vector::operator^(Vector u) {
	return Vector(y*u.z - z * u.y, z*u.x - x * u.z, x*u.y - y * u.x);
}

double Vector::operator*(Vector u)
{
	return x * u.x + y * u.y + z * u.z;
}