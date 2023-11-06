#include "Camera.h"
#include "PPMWriter.h"

#include<iostream>

Camera::Camera() {}

void Camera::render()
{
	PPMWriter::writeHeader(std::cout, width, height);
	for (int j = 0; j < height; j++)
	{
		std::clog << "\rScanlines Remaining: " << (height- j) << " " << std::flush;
		for (int i = 0; i < width; i++)
		{
			auto norm_r = double(i) / (width - 1);
			auto norm_g = double(j) / (height - 1);
			auto norm_b = double(i) / (width- 1);

			Colour pixel_colour;
			pixel_colour.r = 155.999 * norm_r;
			pixel_colour.g = 255.999 * norm_g;
			pixel_colour.b = 255.999 * norm_b;

			PPMWriter::writePixel(std::cout, pixel_colour);
		}
	}
}