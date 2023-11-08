#pragma once

#include "GeoVec.h"

#include <ostream>
class PPMWriter
{
public:
	static void writeHeader(std::ostream& out, int width, int height);
	static void writePixel(std::ostream& out, GeoVec colour);
};

