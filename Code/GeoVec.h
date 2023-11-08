#pragma once
#include "utils.h"
#include <cmath>

class GeoVec
{
    public:
        double x,y,z;

        GeoVec() : x(0), y(0), z(0) {}
        GeoVec(double x, double y, double z) : x(x), y(y), z(z) {}

        GeoVec operator*= (const double& scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        GeoVec operator/= (const double& scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        double length() const
        {
            return sqrt(x*x + y*y + z*z);
        }

        double length_squared() const
        {
            return x*x + y*y + z*z;
        }
};

inline GeoVec operator+ (const GeoVec& v1, const GeoVec& v2)
{
    return GeoVec(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline GeoVec operator+ (const GeoVec& v, const double& scalar)
{
    return GeoVec(v.x + scalar, v.y + scalar, v.z + scalar);
}

inline GeoVec operator- (const GeoVec& v)
{
    return GeoVec(-v.x, -v.y, -v.z);
}
inline GeoVec operator- (const GeoVec& v1, const GeoVec& v2)
{
    return GeoVec(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline GeoVec operator* (const GeoVec& v1, const GeoVec& v2)
{
    return GeoVec(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline GeoVec operator* (const GeoVec& v1, const double& scalar)
{
    return GeoVec(v1.x * scalar, v1.y * scalar, v1.z * scalar);
}

inline GeoVec operator* (const double& scalar, const GeoVec& v1)
{
    return GeoVec(v1.x * scalar, v1.y * scalar, v1.z * scalar);
}

inline GeoVec operator/ (const GeoVec& v1, const double& scalar)
{
    return GeoVec(v1.x / scalar, v1.y / scalar, v1.z / scalar);
}

inline double dot(const GeoVec& v1, const GeoVec& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline GeoVec cross(const GeoVec& v1, const GeoVec& v2)
{
    return GeoVec(v1.y * v2.z - v1.z * v2.y,
                  v1.z * v2.x - v1.x * v2.z,
                  v1.x * v2.y - v1.y * v2.x);
}

inline GeoVec normalize(const GeoVec& v)
{
    return v / v.length();
}

// Write a function that generates a random vector
inline GeoVec random(double min=0, double max=1)
{
    return GeoVec(rand_double(min, max), rand_double(min, max), rand_double(min, max));
}

// write a function that generates a random vector in a unit sphere
inline GeoVec random_in_unit_sphere()
{
    while(true)
    {
        auto p = random(-1, 1);
        if (p.length_squared() < 1.0)
            return p;
    }
}

// write a function that generates a random vector in the same hemisphere as normal
inline GeoVec random_in_hemisphere(const GeoVec& normal)
{
    GeoVec in_unit_sphere = random_in_unit_sphere();
    if(dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}


// write a function that calculates reflection vector
inline GeoVec reflect(const GeoVec& v, const GeoVec& n)
{
    return v - 2*dot(v,n)*n;
}

inline GeoVec refract(const GeoVec& uv, const GeoVec& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    GeoVec r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    GeoVec r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}