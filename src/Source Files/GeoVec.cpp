#include "../Header Files/GeoVec.h"

#include<cmath>

GeoVec::GeoVec()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

GeoVec::GeoVec(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

GeoVec GeoVec::operator+(const GeoVec& v)
{
	return GeoVec(x + v.x, y + v.y, z + v.z);
}

GeoVec GeoVec::operator-(const GeoVec& v)
{
	return GeoVec(x - v.x, y - v.y, z - v.z);
}

GeoVec GeoVec::operator*(const GeoVec& v)
{
	return GeoVec(x * v.x, y * v.y, z * v.z);
}

GeoVec GeoVec::operator/(const GeoVec& v)
{
	return GeoVec(x / v.x, y / v.y, z / v.z);
}

GeoVec GeoVec::operator+(const double& f)
{
	return GeoVec(x + f, y + f, z + f);
}

GeoVec GeoVec::operator-(const double& f)
{
	return GeoVec(x - f, y - f, z - f);
}

GeoVec GeoVec::operator*(const double& f)
{
	return GeoVec(x * f, y * f, z * f);
}

GeoVec GeoVec::operator/(const double& f)
{
	return GeoVec(x / f, y / f, z / f);
}

GeoVec GeoVec::operator+=(const GeoVec& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}	

GeoVec GeoVec::operator-=(const GeoVec& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

GeoVec GeoVec::operator*=(const GeoVec& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

GeoVec GeoVec::operator/=(const GeoVec& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

GeoVec GeoVec::operator+=(const double& f)
{
	x += f;
	y += f;
	z += f;
	return *this;
}

GeoVec GeoVec::operator-=(const double& f)
{
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

GeoVec GeoVec::operator*=(const double& f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

GeoVec GeoVec::operator/=(const double& f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

bool GeoVec::operator==(const GeoVec& v)
{
	return (x == v.x && y == v.y && z == v.z);
}

bool GeoVec::operator!=(const GeoVec& v)
{
	return (x != v.x || y != v.y || z != v.z);
}

double GeoVec::magnitude()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

double GeoVec::dot(const GeoVec& v)
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

GeoVec GeoVec::normalize()
{
	double mag = magnitude();
	return GeoVec(x / mag, y / mag, z / mag);
}

GeoVec GeoVec::cross(const GeoVec& v)
{
	return GeoVec((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x));
}