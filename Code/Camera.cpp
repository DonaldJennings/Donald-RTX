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


void Camera::render(World& scene_hittables, RenderMode render_mode)
{
	refresh();
	PPMWriter::writeHeader(std::cout, width, height);
	for (int j = 0; j < height; ++j)
	{
		std::clog << "\rScanlines Remaining: " << (height- j) << " " << std::flush;
		for (int i = 0; i < width; ++i)
		{
			auto pixel_centre = pixel_origin + (i * horizontal_pixel_change) + (j * vertical_pixel_change);
			auto direction = pixel_centre - camera_pos;
			Ray ray(camera_pos, direction);
			GeoVec colour = render_mode == RenderMode::BINARY ? compute_binary_colour(ray, scene_hittables) : computeColour(ray, scene_hittables);
			PPMWriter::writePixel(std::cout, colour);
		}
	}
}

GeoVec Camera::computeColour(Ray& ray, World& world)
{
	HitRecord hit_record;
	if (world.hit(ray, Interval(0, INFINITY), hit_record))
	{
		GeoVec normal = hit_record.normal;
		return 0.5 * (normal + 1.0);
	}
	return world.backgroundColour;
}

// write a function to perform billphong rendering
