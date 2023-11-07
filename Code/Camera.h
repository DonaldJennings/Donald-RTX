#pragma once
#include "GeoVec.h"
#include "World.h"
#include "Ray.h"
#include "PPMWriter.h"
#include "RenderMode.h"

class Camera
{
	public:
		Camera();
		void render(World& scene_hittables, RenderMode render_mode);
		void setPosition(GeoVec camera_pos) { camera_pos = camera_pos; };
		void setWidth(int width) { this->width = width; };
		void setHeight(int height) { this->height = height; };
		void setFOV(double fov) { this->fov = fov; };
		void look_at(GeoVec look_at) { this->look_at_vec = look_at; };
		void set_up(GeoVec up_vector) { this->up_vector = up_vector; };
	
	private:
		// Camera Settigns
		GeoVec camera_pos;
		GeoVec look_at_vec;
		GeoVec up_vector;
		double exposure;
		double fov;
		double aspect_ratio;
		double focal_length;

		GeoVec u,v,w;
		// Viewport
		GeoVec pixel_origin;
		GeoVec viewport_u;
		GeoVec viewport_v;
		GeoVec horizontal_pixel_change;
		GeoVec vertical_pixel_change;
		GeoVec upper_left;
		int width;
		int height;
		double viewport_height;
		double viewport_width;

		GeoVec computeColour(Ray& ray, World& scene_hittables);
		void refresh();

};