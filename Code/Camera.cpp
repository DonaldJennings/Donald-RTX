#include "Camera.h"
#include<iostream>
#include<limits>
#include<cmath>
#include<math.h>
#include<thread>
#include<chrono>
#include<random>
#include"utils.h"
#include<algorithm>


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
    auto startTimer = std::chrono::high_resolution_clock::now();
    refresh_viewport_settings();
    PPMWriter::writeHeader(std::cout, width, height);

    std::vector<std::thread> threads;
    std::vector<GeoVec> pixel_colours(width * height);

    std::clog << "Rendering " << width << "x" << height << " image" << std::endl;
    auto render_row = [&](int j) {
        for (int i = 0; i < width; ++i)
        {
            GeoVec pixel_color(0, 0, 0);

            if (render_mode.get_name() == "pathtrace")
            {
                auto samples = get_pixel_samples(i, j, num_samples);
                for (auto& ray : samples)
                {
                    pixel_color += render_mode.compute_colour(*ray, world, num_bounces);
                }
                pixel_color /= num_samples;
            }
            else
            {
                Ray ray = ray_from_pixel(i, j);
                pixel_color = render_mode.compute_colour(ray, world, num_bounces);
            }
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
        PPMWriter::writePixel(std::cout, pixel_color, exposure);
    }

    // print render time in 0.00s format
    auto endTimer = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTimer - startTimer;
    std::clog << "\rRender time: " << std::fixed << std::setprecision(2) << elapsed.count() << "seconds";

}

Ray Camera::ray_from_pixel(int i, int j)
{
    GeoVec ray_origin = camera_pos;
    GeoVec ray_direction = pixel_origin + i*horizontal_pixel_change + j*vertical_pixel_change - camera_pos;
    return Ray(ray_origin, normalize(ray_direction));
}

Ray Camera::sample_ray_from_pixel(int i, int j)
{
    // Generate a random ray for the pixel (i,j)
    auto pixel_centre = pixel_origin + i*horizontal_pixel_change + j*vertical_pixel_change;
    auto random_point = pixel_centre + random_double()*horizontal_pixel_change + random_double()*vertical_pixel_change;
    return Ray(camera_pos, normalize(random_point - camera_pos));

}

std::vector<std::shared_ptr<Ray>> Camera::get_pixel_samples(int i, int j, int num_samples)
{
    std::vector<std::shared_ptr<Ray>> samples;
    for (int s = 0; s < num_samples; ++s)
    {
        samples.push_back(std::make_shared<Ray>(sample_ray_from_pixel(i, j)));
    }
    return samples;
}
