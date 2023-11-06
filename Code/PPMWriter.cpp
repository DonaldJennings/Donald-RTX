#include "PPMWriter.h"
#include<iostream>

void PPMWriter::writeHeader(std::ostream& out, int width, int height)
{
	out << "P3\n" << width << ' ' << height << "\n255\n";
}

void PPMWriter::writePixel(std::ostream& out, GeoVec colour)
{
	out << static_cast<int>(255.999 * colour.x) << ' '
		<< static_cast<int>(255.999 * colour.y) << ' '
		<< static_cast<int>(255.999 * colour.z) << '\n';

}