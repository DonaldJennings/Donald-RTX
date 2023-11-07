#include "GeoVec.h"

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
	// return unit vector
	return *this / magnitude();
}