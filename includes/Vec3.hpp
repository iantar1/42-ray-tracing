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

    ~Vec3();
};

using Points3 = Vec3;

Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    return  Vec3(v1.getX() + v2.getX(), v1.getY() + v2.getY(), v1.getZ() + v2.getZ());

}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    return  Vec3(v1.getX() - v2.getX(), v1.getY() - v2.getY(), v1.getZ() - v2.getZ());

}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    return  Vec3(v1.getX() * v2.getX(), v1.getY() * v2.getY(), v1.getZ() * v2.getZ());
}

double  Vec3::getX() const {return this->x;}
double  Vec3::getY() const {return this->y;}
double  Vec3::getZ() const {return this->z;}


Vec3::Vec3(double x, double y, double z):x(x), y(y), z(z)
{
}

Vec3::Vec3():x(0), y(0), z(0)
{
}
Vec3::~Vec3()
{
}
