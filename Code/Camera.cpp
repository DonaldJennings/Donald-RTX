#include "Camera.h"
#include "PPMWriter.h"
#include "Ray.h"

#include<iostream>

Camera::Camera() {}

void Camera::render()
{
	PPMWriter::writeHeader(std::cout, width, height);
	for (int j = 0; j < height; ++j)
	{
		std::clog << "\rScanlines Remaining: " << (height- j) << " " << std::flush;
		for (int i = 0; i < width; ++i)
		{
			GeoVec pixel_centre = pixel_origin + (horizontal_pixel_change * i) + (vertical_pixel_change * j);
			GeoVec direction = pixel_centre - camera_pos;
			Ray ray(camera_pos, direction);
			GeoVec color = ray.colour();
			PPMWriter::writePixel(std::cout, color);
		}
	}
}