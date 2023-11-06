#pragma once
#include "GeoVec.h"

class Camera
{
	public:
	Camera();
	void render();

	private:
		// Camera Settings
		double aspect_ratio = 16.0 / 9.0;
		int width = 1920;
		int height = static_cast<int>(width / aspect_ratio);
		
		// Camera Position Settings
		GeoVec camera_pos = GeoVec(0,0,0); // x=0, y=0, z=0
		
		// Determine viewport vectors


		// Viewport Settings
		double viewport_height = 2.0;
		double viewport_width = viewport_height * (width / height);

		// Horizontal viewport vecotr
		GeoVec horizontal = GeoVec(viewport_width, 0, 0);
		GeoVec vertical = GeoVec(0, -viewport_height, 0);

		// Pixel change vectors
		GeoVec horizontal_pixel_change = horizontal / (width);
		GeoVec vertical_pixel_change = vertical / (height);

		// Upper left pixel location
		GeoVec upper_left = camera_pos - GeoVec(0, 0, 1) - horizontal/2 - vertical/2;
		GeoVec pixel_origin = upper_left + 0.5 * horizontal_pixel_change + vertical_pixel_change;
};