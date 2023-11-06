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
		int width = 720;
		int height = static_cast<int>(width / aspect_ratio);
		
		// Camera Position Settings
		GeoVec camera_pos = GeoVec(0,0,0); // x=0, y=0, z=0
		
		// Determine viewport vectors


		// Viewport Settings
		double viewport_height = 2.0;
		double viewport_width = viewport_height * (width / height);
};

