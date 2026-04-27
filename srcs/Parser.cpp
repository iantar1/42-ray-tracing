#include "../includes/Parser.hpp"
#include "../includes/Sphere.hpp"
#include "../includes/Plane.hpp"
#include "../includes/Cylinder.hpp"
#include "../includes/Cone.hpp"
#include "../includes/Square.hpp"
#include "../includes/Triangle.hpp"
#include "../includes/Light.hpp"
#include "../includes/Camera.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::string& filename)
    : width(800), height(600),
      ambient_intensity(0.1), ambient_color(Vec3(255, 255, 255)),
      cam_pos(Vec3(0, 0, 0)), cam_dir(Vec3(0, 0, 1)), cam_fov(60.0)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        parseLine(line);
    }
}

Vec3 Parser::parseVec3(const std::string& s)
{
    std::string tmp = s;
    for (char& c : tmp)
        if (c == ',') c = ' ';
    std::istringstream ss(tmp);
    double x = 0, y = 0, z = 0;
    ss >> x >> y >> z;
    return Vec3(x, y, z);
}

void Parser::parseLine(const std::string& line)
{
    std::istringstream ss(line);
    std::string type;
    ss >> type;

    if (type == "R")
    {
        ss >> width >> height;
    }
    else if (type == "A")
    {
        std::string color_str;
        ss >> ambient_intensity >> color_str;
        ambient_color = parseVec3(color_str);
    }
    else if (type == "c")
    {
        std::string pos_str, dir_str;
        ss >> pos_str >> dir_str >> cam_fov;
        cam_pos = parseVec3(pos_str);
        cam_dir = parseVec3(dir_str);
    }
    else if (type == "l")
    {
        std::string pos_str, color_str;
        double intensity;
        ss >> pos_str >> intensity >> color_str;
        LightData ld;
        ld.position  = parseVec3(pos_str);
        ld.intensity = intensity;
        lights.push_back(ld);
    }
    else if (type == "sp")
    {
        std::string center_str, color_str;
        double radius;
        ss >> center_str >> radius >> color_str;
        Vec3 center = parseVec3(center_str);
        Vec3 color  = parseVec3(color_str);
        objects.push_back(std::make_unique<Sphere>(radius, center, color));
    }
    else if (type == "pl")
    {
        std::string point_str, normal_str, color_str;
        ss >> point_str >> normal_str >> color_str;
        Vec3 point  = parseVec3(point_str);
        Vec3 normal = parseVec3(normal_str);
        Vec3 color  = parseVec3(color_str);
        objects.push_back(std::make_unique<Plane>(normal, point, color));
    }
    else if (type == "sq")
    {
        std::string center_str, normal_str, color_str;
        double half_size;
        ss >> center_str >> normal_str >> half_size >> color_str;
        Vec3 center = parseVec3(center_str);
        Vec3 normal = parseVec3(normal_str);
        Vec3 color  = parseVec3(color_str);
        objects.push_back(std::make_unique<Square>(center, normal, half_size, color));
    }
    else if (type == "tr")
    {
        std::string v0_str, v1_str, v2_str, color_str;
        ss >> v0_str >> v1_str >> v2_str >> color_str;
        Vec3 v0    = parseVec3(v0_str);
        Vec3 v1    = parseVec3(v1_str);
        Vec3 v2    = parseVec3(v2_str);
        Vec3 color = parseVec3(color_str);
        objects.push_back(std::make_unique<Triangle>(v0, v1, v2, color));
    }
    else if (type == "cy")
    {
        std::string center_str, axis_str, color_str;
        double radius, full_height;
        ss >> center_str >> axis_str >> radius >> full_height >> color_str;
        Vec3 center = parseVec3(center_str);
        Vec3 axis   = parseVec3(axis_str);
        Vec3 color  = parseVec3(color_str);
        objects.push_back(std::make_unique<Cylinder>(center, axis, radius, full_height / 2.0, color));
    }
    else if (type == "cn")
    {
        std::string apex_str, axis_str, color_str;
        double half_angle, full_height;
        ss >> apex_str >> axis_str >> half_angle >> full_height >> color_str;
        Vec3 apex  = parseVec3(apex_str);
        Vec3 axis  = parseVec3(axis_str);
        Vec3 color = parseVec3(color_str);
        objects.push_back(std::make_unique<Cone>(apex, axis, half_angle, full_height / 2.0, color));
    }
}

void Parser::apply(Scene& scene)
{
    scene.camera = Camera(cam_pos, cam_dir, cam_fov, (double)width / height);
    scene.camera.setDimensions(width, height);

    for (const LightData& ld : lights)
        scene.addLight(Light(ld.intensity, 0.0, ld.position));

    for (auto& obj : objects)
        scene.addObject(std::move(obj));
    objects.clear();
}
