#include "Hittable.h"
#include "BoundingBox.h"
#include <algorithm>
#include <random>

class BVHNode : public Hittable
{
public:
    BVHNode(std::vector<std::shared_ptr<Hittable>> shapes, double time0, double time1)
    {
        // Choose axis
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);
        int axis = dis(gen);

        // Sort shapes
        std::sort(shapes.begin(), shapes.end(), [axis](const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
            return a->bounding_box().min()[axis] < b->bounding_box().min()[axis];
        });

        // Create node
        if (shapes.size() == 1)
        {
            left = shapes[0];
            right = shapes[0];
        }
        else if (shapes.size() == 2)
        {
            left = shapes[0];
            right = shapes[1];
        }
        else
        {
            left = std::make_shared<BVHNode>(std::vector<std::shared_ptr<Hittable>>(shapes.begin(), shapes.begin() + shapes.size() / 2), time0, time1);
            right = std::make_shared<BVHNode>(std::vector<std::shared_ptr<Hittable>>(shapes.begin() + shapes.size() / 2, shapes.end()), time0, time1);
        }

        // Create bounding box
        BoundingBox left_box = left->bounding_box();
        BoundingBox right_box = right->bounding_box();

        if (left_box.min().x == std::numeric_limits<double>::max() || right_box.min().x == std::numeric_limits<double>::max())
        {
            std::clog << "No bounding box in BVHNode constructor.\n";
        }

        box = BoundingBox(GeoVec::min(left_box.min(), right_box.min()), GeoVec::max(left_box.max(), right_box.max()));
    }

    BoundingBox bounding_box() const override
    {
        return box;
    }

    bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const override
    {
        if (!box.hit(r, ray_interval, rec))
            return false;

        HitRecord left_rec, right_rec;
        bool hit_left = left->hit(r, ray_interval, left_rec);

        Interval new_interval = ray_interval;
        if (hit_left)
            new_interval.max = left_rec.t;

        bool hit_right = right->hit(r, new_interval, right_rec);

        if (hit_left && hit_right) {
            if (left_rec.t < right_rec.t)
                rec = left_rec;
            else
                rec = right_rec;
            return true;
        } else if (hit_left) {
            rec = left_rec;
            return true;
        } else if (hit_right) {
            rec = right_rec;
            return true;
        } else {
            return false;
        }
    }

    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    BoundingBox box;
};