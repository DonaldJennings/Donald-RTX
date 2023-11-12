#pragma once
#include "World.h"
#include "Ray.h"
#include "PPMWriter.h"
#include "RenderMode.h"
#include "GeoVec.h"
#include "Interval.h"
#include <mutex>

class Camera
{
	public:
		Camera();
		void render(World& scene_hittables, RenderMode& render_mode);
		void set_width(int width) { this->width = width; };
		void set_height(int height) { this->height = height; };
		void set_exposure(double exposure) { this->exposure = exposure; };
		void set_num_bounces(int num_bounces) { this->num_bounces = num_bounces; };
		void go_to(GeoVec camera_pos) { this->camera_pos = camera_pos; };
		void setFOV(double fov) { this->fov = fov; };
		void look_at(GeoVec look_at) { this->look_at_vec = look_at; };
		void set_up(GeoVec up_vector) { this->up_vector = up_vector; };
		void render_multithreaded(World& world, RenderMode& render_mode, int j);

	private:

		// Camera Settigns
		std::mutex mtx;
		GeoVec camera_pos;
		GeoVec look_at_vec;
		GeoVec up_vector;
		double exposure;
		double fov;
		double aspect_ratio;
		double focal_length;
		int num_bounces;

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

		Ray ray_from_pixel(int i, int j);
		Ray sample_ray_from_pixel(int i, int j);
		void refresh_viewport_settings();

};