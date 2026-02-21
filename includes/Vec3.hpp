# pragma once

class Vec3
{
private:
    double x;
    double y;
    double z;

public:
    Vec3(double, double, double);
    Vec3();

    double getX() const;
    double getY() const;
    double getZ() const;


    void setX(double x);
    void setY(double y);
    void setZ(double z);

    static double dot(const Vec3& v1, const Vec3& v2);

    ~Vec3();
    
    Vec3 operator+(const Vec3& other)
    {
        return Vec3(this->x + other.getX(), this->y + other.getY(), this->z + other.getZ());
    }

    Vec3 operator-(const Vec3& other)
    {
        return Vec3(this->x - other.getX(), this->y - other.getY(), this->z - other.getZ());
    }

    Vec3 operator*(double scalar)
    {
        return Vec3(this->x * scalar, this->y * scalar, this->z * scalar);
    }

    static Vec3 cross(const Vec3& v1, const Vec3& v2)
    {
        return Vec3(
            v1.getY() * v2.getZ() - v1.getZ() * v2.getY(),
            v1.getZ() * v2.getX() - v1.getX() * v2.getZ(),
            v1.getX() * v2.getY() - v1.getY() * v2.getX()
        );
    }
};
using Points3 = Vec3;

