#include "PPMWriter.h"
#include "Interval.h"
#include<iostream>

void PPMWriter::writeHeader(std::ostream& out, int width, int height)
{
	out << "P3\n" << width << ' ' << height << "\n255\n";
}


void PPMWriter::writePixel(std::ostream& out, GeoVec colour, double exposure)
{	
		
	colour.x = 1 - exp(-colour.x * exposure);
	colour.y = 1 - exp(-colour.y * exposure);
	colour.z = 1 - exp(-colour.z * exposure);


	double gamma = 2.2;  // Adjust this value as needed.
	colour.x = pow(colour.x, 1.0 / gamma);
	colour.y = pow(colour.y, 1.0 / gamma);
	colour.z = pow(colour.z, 1.0 / gamma);

	static const Interval intensity = Interval(0, 0.999);
	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * intensity.clamp(colour.x)) << ' '
		<< static_cast<int>(256 * intensity.clamp(colour.y)) << ' '
		<< static_cast<int>(256 * intensity.clamp(colour.z)) << '\n';

}