#pragma once
#include "utils.h"
#include "World.h"
#include "Ray.h"
#include "PPMWriter.h"
#include "RenderMode.h"
#include "GeoVec.h"
#include "Interval.h"

class Camera
{
	public:
		Camera();
		void render(World& scene_hittables, RenderMode& render_mode);
		void set_width(int width) { this->width = width; };
		void set_height(int height) { this->height = height; };
		void go_to(GeoVec camera_pos) { this->camera_pos = camera_pos; };
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

		// Rendering
		int max_bounce_depth=10;
		GeoVec computeColour(Ray& ray, int bounce_depth, World& scene_hittables);

		GeoVec compute_blinn_phong_colour(Ray& ray, int bounce_depth, World& world);
		GeoVec compute_binary_colour(Ray& ray, int bounce_depth, World& world);
		void refresh();

		Ray get_ray(int i, int j);
		GeoVec pixel_sample_square();

};