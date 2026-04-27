#pragma once
#include "Ray.hpp"

class Objects
{
private:
    Vec3 color;
    double shininess;  // Exponent for specular highlight (higher = shinier/sharper)
    double transparency;  // Transparency value (0.0 = opaque, 1.0 = fully transparent)
    double reflectivity;  // Reflectivity value (0.0 = non-reflective, 1.0 = fully reflective)

public:
    Objects(/* args */);
    Objects(const Vec3& _color);
    Objects(const Vec3& _color, double _shininess);
    Objects(const Vec3& _color, double _shininess, double _transparency);
    Objects(const Vec3& _color, double _shininess, double _transparency, double _reflectivity);
    virtual ~Objects();

    Vec3 getColor() const;
    void setColor(const Vec3& _color);
    double getShininess() const;
    void setShininess(double _shininess);
    double getTransparency() const;
    void setTransparency(double _transparency);
    double getReflectivity() const;
    void setReflectivity(double _reflectivity);

    virtual bool intersect(const Ray& ray, double& t_hit) = 0;
    virtual Vec3 get_normal(const Points3& hit) = 0;
    virtual void translate(const Vec3& offset) = 0;
    virtual void rotateX(double angle) = 0;
    virtual void rotateY(double angle) = 0;
    virtual void rotateZ(double angle) = 0;

};
