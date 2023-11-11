#pragma once
// <<COPILOT>> - Define an axis-aligned bounding box class using GeoVec
#include "GeoVec.h"
#include "Hittable.h"
#include "Interval.h"
#include <limits>

class BoundingBox : Hittable 
{
public:
    BoundingBox() 
        : min_(GeoVec(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max())),
          max_(GeoVec(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest())) 
    {}

    BoundingBox(GeoVec a, GeoVec b) : min_(a), max_(b) {}

    bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const override
    {
        for (int a = 0; a < 3; a++)
        {
            auto invD = 1.0f / r.direction[a];
            auto t0 = (min_[a] - r.origin[a]) * invD;
            auto t1 = (max_[a] - r.origin[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            ray_interval.min = t0 > ray_interval.start() ? t0 : ray_interval.start();
            ray_interval.max = t1 < ray_interval.end() ? t1 : ray_interval.end();
            if (ray_interval.end() <= ray_interval.start())
                return false;
        }
        return true;   
    }

    BoundingBox bounding_box() const override
    {
        return *this;
    }

    void expand(const GeoVec& point) 
    {
        min_ = GeoVec::min(min_, point);
        max_ = GeoVec::max(max_, point);
    }

    void expand(const BoundingBox& other) {
        min_ = GeoVec::min(min_, other.min_);
        max_ = GeoVec::max(max_, other.max_);
    }

    GeoVec center() const {
        return (min_ + max_) * 0.5f;
    }

    GeoVec size() const {
        return max_ - min_;
    }

    GeoVec min() const {
        return min_;
    }

    GeoVec max() const {
        return max_;
    }

private:
    GeoVec min_;
    GeoVec max_;
};
