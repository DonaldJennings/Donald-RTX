#pragma once
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

inline bool operator== (const GeoVec& v1, const GeoVec& v2)
{
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
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

inline GeoVec operator+= (GeoVec& v1, const GeoVec& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

inline GeoVec normalize(const GeoVec& v)
{
    return v / v.length();
}

inline GeoVec reflect(const GeoVec& v, const GeoVec& n)
{
    return v - 2 * dot(v, n) * n;
}

inline bool refract(const GeoVec& v, const GeoVec& n, double ni_over_nt, GeoVec& refracted)
{
    GeoVec uv = normalize(v);
    double dt = dot(uv, n);
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}