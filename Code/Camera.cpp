#include "Camera.h"
#include<iostream>
#include<limits>
#include<cmath>
#include<math.h>
#include<thread>
#include<chrono>
#include<random>
#include<algorithm>


double pi = 3.14159265358979323846;


double random_double() 
{
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
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
    auto startTimer = std::chrono::high_resolution_clock::now();
    refresh_viewport_settings();
    PPMWriter::writeHeader(std::cout, width, height);

    std::vector<std::thread> threads;
    std::vector<GeoVec> pixel_colours(width * height);

    auto render_row = [&](int j) {
        for (int i = 0; i < width; ++i)
        {
            GeoVec pixel_color(0, 0, 0);
            int num_samples = 10;
            for (int s = 0; s < num_samples; ++s)
            {
                Ray ray = sample_ray_from_pixel(i, j);
                pixel_color += render_mode.compute_colour(ray, world, num_bounces);
            }
            pixel_color /= num_samples;

            // Apply exposure
            pixel_color.x = 1 - exp(-exposure * pixel_color.x);
            pixel_color.y = 1 - exp(-exposure * pixel_color.y);
            pixel_color.z = 1 - exp(-exposure * pixel_color.z);
            
            pixel_colours[j * width + i] = pixel_color;
        }
        progressCounter++;
        if (progressCounter % 10 == 0)
        {
            std::lock_guard<std::mutex> lock(writeMutex);
            std::clog << "\rScanlines remaining: " << (height - progressCounter) << ' ' << std::flush;
        }
    };

    for (int j = 0; j < height; ++j)
    {
        threads.push_back(std::thread(render_row, j));
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    for (const auto& pixel_color : pixel_colours)
    {
        PPMWriter::writePixel(std::cout, pixel_color);
    }

    std::clog << "Render complete in " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - startTimer).count() << "s" << std::endl;
}

Ray Camera::ray_from_pixel(int i, int j)
{
    GeoVec ray_origin = camera_pos;
    GeoVec ray_direction = pixel_origin + i*horizontal_pixel_change + j*vertical_pixel_change - camera_pos;
    return Ray(ray_origin, ray_direction);
}

Ray Camera::sample_ray_from_pixel(int i, int j)
{
    // Generate a random ray for the pixel (i,j)
    auto pixel_centre = pixel_origin + i*horizontal_pixel_change + j*vertical_pixel_change;
    auto random_point = pixel_centre + random_double()*horizontal_pixel_change + random_double()*vertical_pixel_change;
    return Ray(camera_pos, normalize(random_point - camera_pos));

}
