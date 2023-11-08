#include "PPMWriter.h"
#include "Interval.h"
#include<iostream>

void PPMWriter::writeHeader(std::ostream& out, int width, int height)
{
	out << "P3\n" << width << ' ' << height << "\n255\n";
}

void PPMWriter::writePixel(std::ostream& out, GeoVec colour)
{
	auto r = colour.x;
    auto g = colour.y;
    auto b = colour.z;

    // Divide the color by the number of samples.
    auto scale = 1.0 / 5;
    r *= scale;
    g *= scale;
    b *= scale;

    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);
    
    // Write the translated [0,255] value of each color component.
    static const Interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}