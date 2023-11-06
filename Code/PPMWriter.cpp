#include "PPMWriter.h"
#include "Colour.h"
#include<iostream>

void PPMWriter::writeHeader(std::ostream& out, int width, int height)
{
	out << "P3\n" << width << ' ' << height << "\n255\n";
}

void PPMWriter::writePixel(std::ostream& out, Colour colour)
{
	out << static_cast<int>(colour.r) << ' '
		<< static_cast<int>(colour.g) << ' '
		<< static_cast<int>(colour.b) << '\n';

}