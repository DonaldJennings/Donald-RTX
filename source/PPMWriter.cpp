#include "PPMWriter.h"
#include<iostream>

void PPMWriter::writeHeader(std::ostream& out, int width, int height)
{
	out << "P3\n" << width << ' ' << height << "\n255\n";
}

void PPMWriter::writePixel(std::ostream& out, GeoVec colour)
{
	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * colour.x) << ' '
		<< static_cast<int>(256 * colour.y) << ' '
		<< static_cast<int>(256 * colour.z) << '\n';

}