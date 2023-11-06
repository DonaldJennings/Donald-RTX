#include "Ray.h"

Ray::Ray(GeoVec origin, GeoVec direction)
{
	this->origin = origin;
	this->direction = direction;
}

GeoVec Ray::at(double t)
{
	return origin + (direction * t);
}