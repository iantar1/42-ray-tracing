#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Scene.hpp"
#include "Objects.hpp"

class Parser
{
public:
    int    width;
    int    height;

    Parser(const std::string& filename);
    void apply(Scene& scene);

private:
    struct LightData {
        Vec3   position;
        double intensity;
    };

    double ambient_intensity;
    Vec3   ambient_color;

    Vec3   cam_pos;
    Vec3   cam_dir;
    double cam_fov;

    std::vector<LightData>              lights;
    std::vector<std::unique_ptr<Objects>> objects;

    Vec3 parseVec3(const std::string& s);
    void parseLine(const std::string& line);
};
