#pragma once
// <<COPILOT>> - Define an axis-aligned bounding box class using GeoVec
#include "GeoVec.h"
#include "Hittable.h"
#include "Interval.h"
#include <limits>

class BoundingBox
{
public:
    BoundingBox() {};

    BoundingBox(const Interval& x, const Interval& y, const Interval& z) : x_(x), y_(y), z_(z) {}

    BoundingBox(const GeoVec& a, const GeoVec& b)
    {
        double epsilon = 0.01;  // Adjust this value as needed.
        x_ = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]) + epsilon);
        y_ = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]) + epsilon);
        z_ = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]) + epsilon);
    }

    BoundingBox(const BoundingBox& box_a, const BoundingBox& box_b)
    {
        x_ = Interval(box_a.x_, box_b.x_);
        y_ = Interval(box_a.y_, box_b.y_);
        z_ = Interval(box_a.z_, box_b.z_);
    }

    GeoVec min() const
    {
        return GeoVec(x_.start(), y_.start(), z_.start());
    }

    GeoVec max() const
    {
        return GeoVec(x_.end(), y_.end(), z_.end());
    }


    Interval get_axis(int axis) const
    {
        if (axis == 2) return z_;
        if (axis == 1) return y_;
        if (axis == 0) return x_;

        std::clog << "Invalid axis" << std::endl;
        return x_;

    }

bool hit(const Ray& r, Interval ray_t) const {
    double epsilon = 0.0001;  // Adjust this value as needed.
    for (int a = 0; a < 3; a++) {
        auto invD = 1 / r.direction[a];
        auto orig = r.origin[a];

        auto t0 = (get_axis(a).start() - orig) * invD - epsilon;
        auto t1 = (get_axis(a).end() - orig) * invD + epsilon;

        if (invD < 0)
            std::swap(t0, t1);

        if (t0 > ray_t.start()) ray_t.min_ = t0;
        if (t1 < ray_t.end()) ray_t.max_ = t1;

        if (ray_t.max_ <= ray_t.min_)
        {
            return false;
        }
    }
    return true;
}

    Interval get_x() const
    {
        return x_;
    }

    Interval get_y() const
    {
        return y_;
    }

    Interval get_z() const
    {
        return z_;
    }
    private:
        Interval x_;
        Interval y_;
        Interval z_;

};