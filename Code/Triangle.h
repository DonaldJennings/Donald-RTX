#pragma once
#include "Hittable.h"
#include "GeoVec.h"

class Triangle : public Hittable {
public:
    Triangle(const GeoVec& v0, const GeoVec& v1, const GeoVec& v2)
        : m_v0(v0), m_v1(v1), m_v2(v2){}

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
        rec.material = mat;

        return true;
    }

private:
    GeoVec m_v0, m_v1, m_v2;
    Material mat;
};