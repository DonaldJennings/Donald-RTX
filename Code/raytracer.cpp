#include "Camera.h"
#include "World.h"
#include "Sphere.h"
#include "nlohmann/json.hpp"
#include "Ray.h"
#include "utils.h"
#include "RenderMode.h"
#include "Cylinder.h"
#include "GeoVec.h"
#include "BinaryRender.h"
#include "BHVNode.h"
#include "BlinnPhong.h"
#include "Triangle.h"
#include "Material.h"
#include "Pathtrace.h"
#include "Texture.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

json parseScene(const char* filename)
{
	// open file
	std::ifstream scene_file(filename);
	if (!scene_file.is_open())
	{
		std::cerr << "Error: Could not open scene file" << std::endl;
		return json();
	}
	// parse scene
	json scene;
	scene_file >> scene;
	return scene;
}


void add_world_lights(World& world, json& parsed_scene)
{
	for (auto& light : parsed_scene["scene"]["lightsources"])
	{
		auto position = GeoVec(light["position"][0], light["position"][1], light["position"][2]);
		auto intensity = GeoVec(light["intensity"][0], light["intensity"][1], light["intensity"][2]);

		world.add_light(std::make_shared<Light>(position, intensity));
	}
	std::clog << "Lights setup complete" << std::endl;
}

void add_hittable_sphere(std::vector<std::shared_ptr<Hittable>>& shapes, json& parsed_sphere, Material& material)
{
	auto center = GeoVec(parsed_sphere["center"][0], parsed_sphere["center"][1], parsed_sphere["center"][2]);
	double radius = parsed_sphere["radius"].get<double>();

	Sphere sphere(center, radius);
	sphere.set_material(material);
	shapes.push_back(std::make_shared<Sphere>(sphere));
}

void add_hittable_cylinder(std::vector<std::shared_ptr<Hittable>>& shapes, json& parsed_cylinder, Material& material)
{
	GeoVec center { GeoVec(parsed_cylinder["center"][0], parsed_cylinder["center"][1], parsed_cylinder["center"][2]) };
	GeoVec axis { normalize(GeoVec(parsed_cylinder["axis"][0], parsed_cylinder["axis"][1], parsed_cylinder["axis"][2])) };
	double radius { parsed_cylinder["radius"].get<double>() };
	double height { parsed_cylinder["height"].get<double>() };
	std::clog << "Cylinder Details: " << std::endl;
	std::clog << "Center: " << center << std::endl;
	std::clog << "Axis: " << axis << std::endl;
	std::clog << "Radius: " << radius << std::endl;
	std::clog << "Height: " << height << std::endl;
	
	Cylinder cylinder(center, axis, radius, height);
	cylinder.set_material(material);
	shapes.push_back(std::make_shared<Cylinder>(cylinder));
}

void add_hittable_triangle(std::vector<std::shared_ptr<Hittable>>& shapes, json& parsed_triangle, Material& material)
{
	auto v0 { GeoVec(parsed_triangle["v0"][0], parsed_triangle["v0"][1], parsed_triangle["v0"][2]) };
	auto v1 { GeoVec(parsed_triangle["v1"][0], parsed_triangle["v1"][1], parsed_triangle["v1"][2]) };
	auto v2 { GeoVec(parsed_triangle["v2"][0], parsed_triangle["v2"][1], parsed_triangle["v2"][2]) };

	Triangle triangle(v0, v1, v2);
	triangle.set_material(material);
	shapes.push_back(std::make_shared<Triangle>(triangle));
}

Material parse_material(json& mat_json)
{	
	double ks = mat_json["ks"].get<double>();
	double kd = mat_json["kd"].get<double>();
	int specularexponent = mat_json["specularexponent"].get<int>();
	GeoVec diffusecolor = GeoVec(mat_json["diffusecolor"][0], mat_json["diffusecolor"][1], mat_json["diffusecolor"][2]);
	GeoVec specularcolor = GeoVec(mat_json["specularcolor"][0], mat_json["specularcolor"][1], mat_json["specularcolor"][2]);
	bool isreflective = mat_json["isreflective"].get<bool>();
	bool isrefractive = mat_json["isrefractive"].get<bool>();
	double reflectivity = mat_json["reflectivity"].get<double>();
	double refractiveindex = mat_json["refractiveindex"].get<double>();
	double roughness = 1;
	double fresnel = 0.1;
	if (mat_json.find("roughness") != mat_json.end())
	{
		std::clog << "Roughness found" << std::endl;
		double roughness = mat_json["roughness"].get<double>();
	}

	if (mat_json.find("fresnel") != mat_json.end())
	{
		std::clog << "Fresnel found" << std::endl;
		double fresnel = mat_json["fresnel"].get<double>();
	}


	// Extract texture information
	Texture texture;
	if (mat_json.find("texture") != mat_json.end())
	{
		std::clog << "Texture found" << std::endl;
		Texture texture("../Textures/" + mat_json["texture"].get<std::string>());

		return Material(ks, kd, specularexponent, diffusecolor, specularcolor, isreflective, reflectivity, isrefractive, refractiveindex, texture, roughness, fresnel);
	}
	return Material(ks, kd, specularexponent, diffusecolor, specularcolor, isreflective, reflectivity, isrefractive, refractiveindex, roughness, fresnel);
}
void setup_camera(Camera& camera, json& parsed_camera, int num_bounces, int num_samples)
{
	camera.setFOV(parsed_camera["fov"]);
	camera.set_width(parsed_camera["width"]);
	camera.set_height(parsed_camera["height"]);
	camera.set_exposure(parsed_camera["exposure"]);

	// std::clog << "Setting number of bounces to " << num_bounces << std::endl;

	camera.set_num_bounces(num_bounces);
	camera.set_num_samples(num_samples);

	if (parsed_camera.find("aperture") != parsed_camera.end())
	{
		std::clog << "Aperture found" << std::endl;
		camera.set_aperature(parsed_camera["aperture"]);
		std::clog << "Aperture set to " << parsed_camera["aperture"] << std::endl;
	} else {
		camera.set_aperature(0.01);
		std::clog << "Aperture to default value of 0.01" << std::endl;
	}

	camera.go_to(GeoVec(parsed_camera["position"][0], parsed_camera["position"][1], parsed_camera["position"][2]));
	camera.look_at(GeoVec(parsed_camera["lookAt"][0], parsed_camera["lookAt"][1], parsed_camera["lookAt"][2]));
	camera.set_up(GeoVec(parsed_camera["upVector"][0], parsed_camera["upVector"][1], parsed_camera["upVector"][2]));
	std::clog << "Camera setup complete" << std::endl;
}





int main(int argc, char** argv)
{

	// Get first argument and store as scene file directory
	std::string scene_file_dir = argv[1];
	std::clog << "Scene file: " << scene_file_dir << std::endl;
	json parsed_scene = parseScene(scene_file_dir.c_str());

	Camera camera;
	BinaryRender binary_render;
	BlinnPhong blinn_phong;

	std::vector<std::shared_ptr<Hittable>> hittables;
	for (auto& shape : parsed_scene["scene"]["shapes"])
	{
		Material material_obj;
		if (shape.find("material") != shape.end())
		{
			material_obj = parse_material(shape["material"]);
		}

		if (shape["type"] == "sphere")
		{
			add_hittable_sphere(hittables, shape, material_obj);
		}
		else if (shape["type"] == "cylinder")
		{
			add_hittable_cylinder(hittables, shape, material_obj);
		}
		else if (shape["type"] == "triangle")
		{
			add_hittable_triangle(hittables, shape, material_obj);
		}
		else
		{
			std::cerr << "Shape type not recognised" << std::endl;
		}
	}

	std::clog << "Number of shapes: " << hittables.size() << std::endl;
	std::clog << "Shapes setup complete" << std::endl;
	int num_bounces = 8;
	if (parsed_scene["rendermode"] != "binary")
	{
		num_bounces = parsed_scene["nbounces"].get<int>();
		std::clog << "Number of bounces set to " << num_bounces << std::endl;
	}

	// extract nsamples from parsed scene
	int num_samples = 5;
	if (parsed_scene.find("nsamples") != parsed_scene.end())
	{
		num_samples = parsed_scene["nsamples"].get<int>();
		std::clog << "Number of samples set to " << num_samples << std::endl;
	}

	Pathtrace pathtrace;
	setup_camera(camera, parsed_scene["camera"], num_bounces, num_samples);

	std::clog << "Constructing BVH" << std::endl;

	World world_bvh{std::make_shared<BVHNode>(hittables, 0, hittables.size())};
	
	if (parsed_scene["rendermode"] != "binary")
	{
		add_world_lights(world_bvh, parsed_scene);
	}
	std::clog << "Added world lights" << std::endl;
	world_bvh.backgroundColour = GeoVec(parsed_scene["scene"]["backgroundcolor"][0], parsed_scene["scene"]["backgroundcolor"][1], parsed_scene["scene"]["backgroundcolor"][2]);

	std::clog << "BVH constructed" << std::endl;

	// Determine render mode from parsed scene
	if (parsed_scene["rendermode"] == "binary")
	{
		std::clog << "Binary render mode selected" << std::endl;
		camera.render(world_bvh, binary_render);
	}
	else if (parsed_scene["rendermode"] == "pathtrace")
	{
		std::clog << "Pathtrace render mode selected" << std::endl;
		camera.render(world_bvh, pathtrace);
	}
	else if (parsed_scene["rendermode"] == "phong")
	{
		std::clog << "Blinn Phong render mode selected" << std::endl;
		camera.render(world_bvh, blinn_phong);
	}
	else
	{
		std::cerr << "Render mode not recognised" << std::endl;
	}
}