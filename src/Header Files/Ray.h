#include "./GeoVec.h"

#pragma once
class Ray
{
	// A ray R at point t: R(t) = Origin + tDirection
	GeoVec origin;
	GeoVec direction;

	Ray::Ray();
	Ray::Ray(GeoVec origin, GeoVec direction);
	GeoVec Ray::at(double t);
};

