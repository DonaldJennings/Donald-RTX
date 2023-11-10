#include "Camera.h"
#include<iostream>
#include<limits>
#include<cmath>
#include<math.h>

double pi = 3.14159265358979323846;

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

void Camera::refresh_viewport_settings()
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


void Camera::render(World& world, RenderMode& render_mode)
{
	refresh_viewport_settings();
	PPMWriter::writeHeader(std::cout, width, height);
    for (int j = 0; j < height; ++j)
    {
        std::clog << "\rScanlines remaining: " << (height - j) << ' ' << std::flush;
        for (int i = 0; i < width; ++i)
        {
            Ray ray_to_pixel = ray_from_pixel(i, j);
            GeoVec pixel_color = render_mode.compute_colour(ray_to_pixel, world, num_bounces);
            PPMWriter::writePixel(std::cout, pixel_color);
        }
	}
    std::clog << "\nDone.\n";
}


Ray Camera::ray_from_pixel(int i, int j)
{
    GeoVec ray_origin = camera_pos;
    GeoVec ray_direction = pixel_origin + i*horizontal_pixel_change + j*vertical_pixel_change - camera_pos;
    return Ray(ray_origin, ray_direction);
}