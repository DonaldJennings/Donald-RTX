#include "PPMWriter.h"
#include "Interval.h"
#include<iostream>

void PPMWriter::writeHeader(std::ostream& out, int width, int height)
{
	out << "P3\n" << width << ' ' << height << "\n255\n";
}


void PPMWriter::writePixel(std::ostream& out, GeoVec colour)
{	
	colour.x = std::sqrt(colour.x);
	colour.y = std::sqrt(colour.y);
	colour.z = std::sqrt(colour.z);
	
	static const Interval intensity = Interval(0, 0.999);
	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * intensity.clamp(colour.x)) << ' '
		<< static_cast<int>(256 * intensity.clamp(colour.y)) << ' '
		<< static_cast<int>(256 * intensity.clamp(colour.z)) << '\n';

}