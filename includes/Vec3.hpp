# pragma once

class Vec3
{
private:
    double x;
    double y;
    double z;

public:
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec3() : x(0), y(0), z(0) {}

    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }
    void setZ(double z) { this->z = z; }

    static double dot(const Vec3& v1, const Vec3& v2)
    {
        return (v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ());
    }

    ~Vec3() {}
    
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