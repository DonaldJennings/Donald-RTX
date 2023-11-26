#pragma once
#include "Hittable.h"
#include "GeoVec.h"

class Triangle : public Hittable {
public:
    Triangle(const GeoVec& v0, const GeoVec& v1, const GeoVec& v2)
        : m_v0(v0), m_v1(v1), m_v2(v2)
        {
            GeoVec min = GeoVec::min(v0, GeoVec::min(v1, v2));
            GeoVec max = GeoVec::max(v0, GeoVec::max(v1, v2));
            box = BoundingBox(min, max);
        }

    void set_material(Material mat) { this->mat = mat; }

    bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const override
    {
        GeoVec e1 = m_v1 - m_v0;
        GeoVec e2 = m_v2 - m_v0;
        GeoVec pvec = cross(r.direction, e2);
        double det = dot(e1, pvec);

        if (det < 1e-8 && det > -1e-8) {
            return false;
        }

        double inv_det = 1.0 / det;
        GeoVec tvec = r.origin - m_v0;
        double u = dot(tvec, pvec) * inv_det;

        if (u < 0.0 || u > 1.0) {
            return false;
        }

        GeoVec qvec = cross(tvec, e1);
        double v = dot(r.direction, qvec) * inv_det;

        if (v < 0.0 || u + v > 1.0) {
            return false;
        }

        double t = dot(e2, qvec) * inv_det;

        if (!ray_interval.surrounds(t)) {
            return false;
        }

        rec.t = t;
        rec.point = r.at(t);
        GeoVec outward_normal = normalize(cross(e1, e2));
        rec.set_face_normal(r, outward_normal);
        rec.material = std::make_shared<Material>(mat);
        rec.shape = std::make_shared<Triangle>(*this);

        return true;
    }
    
    std::pair<double, double> compute_uv(const HitRecord& rec) const override
    {
        GeoVec v0v1 = m_v1 - m_v0;
        GeoVec v0v2 = m_v2 - m_v0;
        GeoVec v0p = rec.point - m_v0;
        double d00 = dot(v0v1, v0v1);
        double d01 = dot(v0v1, v0v2);
        double d11 = dot(v0v2, v0v2);
        double d20 = dot(v0p, v0v1);
        double d21 = dot(v0p, v0v2);
        double denom = d00 * d11 - d01 * d01;

        if (denom == 0)
        {
            return std::make_pair(0, 0);
        }

        double v = (d11 * d20 - d01 * d21) / denom;
        double w = (d00 * d21 - d01 * d20) / denom;
        double u = 1.0 - v - w;

        // Check if the barycentric coordinates are within the valid range
        if (u < 0 || u > 1 || v < 0 || v > 1)
        {
            std::cerr << "Invalid barycentric coordinates: u=" << u << ", v=" << v << std::endl;
            return std::make_pair(0, 0);
        }

        double scaleFactor = 10.0; // Adjust this value to get the desired density
        u *= scaleFactor;
        v *= scaleFactor;
        return std::make_pair(u, v);
    }
    
    BoundingBox bounding_box() const override
    {
        return box;
    }
    
private:
    GeoVec m_v0, m_v1, m_v2;
    Material mat;
    BoundingBox box;
};