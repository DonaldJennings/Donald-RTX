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

GeoVec Ray::colour()
{
	// Calculate the colour from the direction of the ray
	GeoVec unit_direction = direction.normalize();
	double t = 0.9 * (unit_direction.y + 1.0);
	GeoVec white = GeoVec(1.0, 1.0, 1.0);
	GeoVec blue = GeoVec(0.5, 0.7, 1.0);
	return (1.0-t)*white + t*blue;
}