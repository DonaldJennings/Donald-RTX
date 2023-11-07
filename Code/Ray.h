#include "GeoVec.h"

#pragma once
class Ray
{
	// A ray R at point t: R(t) = Origin + tDirection
	public:
		GeoVec origin;
		GeoVec direction;
		Ray(GeoVec origin, GeoVec direction);
		GeoVec at(double t);
		GeoVec colour();
};

