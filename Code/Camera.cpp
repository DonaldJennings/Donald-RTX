#include "Camera.h"
#include<iostream>
#include<limits>
#include<cmath>
#include<math.h>

double pi = 3.14159265358979323846;
// write function to compute binary colour if ray hits an object
GeoVec compute_binary_colour(Ray& ray, World& world)
{
	HitRecord hit_record;
	if (world.hit(ray, Interval(0.0, INFINITY), hit_record))
	{
		return GeoVec(1, 0, 0);
	}
	return world.backgroundColour;
}

Camera::Camera() 
{
	width = 720;
	aspect_ratio = 16.0 / 9.0;
	height = static_cast<int>(width / aspect_ratio);

	camera_pos = GeoVec(0.0, 0.0, 0.0);
	look_at_vec = GeoVec(0.0, 0.0, 1.0);
	up_vector = GeoVec(0.0, 1.0, 0.0);

	fov = 90;
	max_bounce_depth = 10;
}

void Camera::refresh()
{
	double fov_radians = fov * (pi / 180);
	focal_length = (camera_pos - look_at_vec).length();
    viewport_height = 2.0 * tan(fov_radians / 2) * focal_length;
    viewport_width = viewport_height * (static_cast<double>(width)/height);

	w = normalize(camera_pos - look_at_vec);
	u = normalize(cross(up_vector, w));
	v = cross(w, u);

    viewport_u = viewport_width * -u;
    viewport_v = viewport_height * -v;
    horizontal_pixel_change = viewport_u / width;
    vertical_pixel_change = viewport_v / height;
    upper_left = camera_pos - (focal_length * w) - (0.5 * viewport_u) - (0.5 * viewport_v);
    pixel_origin = upper_left + 0.5 * (horizontal_pixel_change + vertical_pixel_change);
}


void Camera::render(World& scene_hittables, RenderMode& render_mode)
{
	refresh();
	PPMWriter::writeHeader(std::cout, width, height);
	for (int j = 0; j < height; ++j)
	{
		std::clog << "\rScanlines Remaining: " << (height- j) << " " << std::flush;
		for (int i = 0; i < width; ++i)
		{
            GeoVec pixel_color(0,0,0);
            for (int sample = 0; sample < 10; ++sample) {
                Ray r = get_ray(i, j);
                pixel_color = pixel_color + render_mode.compute_colour(r, max_bounce_depth, scene_hittables);
            }
            PPMWriter::writePixel(std::cout, pixel_color);
		}
	}
	std::clog << "\nDone.\n";
}

GeoVec Camera::computeColour(Ray& ray, int bounce_depth, World& world)
{
	HitRecord hit_record;

	if (bounce_depth <= 0)
	{
		return GeoVec(0, 0, 0);
	}

	if (world.hit(ray, Interval(0.001, INFINITY), hit_record))
	{

		// return surface normal

		return 0.5 * (hit_record.normal + GeoVec(1, 1, 1));

	} 
	
    GeoVec unit_direction = normalize(ray.direction);
    auto a = 0.1*(unit_direction.y + 1.0);
    return (1.0-a)*GeoVec(1.0, 1.0, 1.0) + a*GeoVec(0.5, 0.7, 1.0);
}

Ray Camera::get_ray(int i, int j) {
    // Get a randomly sampled camera ray for the pixel at location i,j.
    auto pixel_center = pixel_origin + (i * horizontal_pixel_change) + (j * vertical_pixel_change);
    auto pixel_sample = pixel_center + pixel_sample_square();
    auto ray_origin = camera_pos;
    auto ray_direction = pixel_sample - ray_origin;
    return Ray(ray_origin, ray_direction);
}
GeoVec Camera::pixel_sample_square() {
    // Returns a random point in the square surrounding a pixel at the origin.
    auto px = -0.5 + rand_double();
    auto py = -0.5 + rand_double();
    return (px * horizontal_pixel_change) + (py * vertical_pixel_change);
}