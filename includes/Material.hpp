# pragma once

# include "Ray.hpp"
# include "Hittable.hpp"

class Material
{
private:
    /* data */
public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const {
        return false;
    }
};

class Lambertian : public Material {
  public:
    Lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered)
    const override {
        Vec3 scatter_direction = rec.normal + random_unit_vector();
         // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

  private:
    color albedo;
};

class Metal : public Material {
  public:
    Metal(const color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r_in, const hit_record& rec, color& attenuation,  Ray& scattered)
    const override {
        Vec3 reflected = reflect(r_in.direction(), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return true;
    }

  private:
    color albedo;
};