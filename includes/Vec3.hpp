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
};

using Points3 = Vec3;

