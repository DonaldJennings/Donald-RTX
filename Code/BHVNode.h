#include "Hittable.h"
#include "BoundingBox.h"
#include "World.h"
#include "Interval.h"
#include "Triangle.h"
#include <algorithm>
#include <random>

class BVHNode : public Hittable
{
public:
    BVHNode(const World& world) : BVHNode(world.hittables, 0, world.hittables.size()) {}

    BVHNode(const std::vector<std::shared_ptr<Hittable>>& shapes, size_t start, size_t end)
    {
        auto shapes_copy = shapes;
        // Choose axis
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);
        int axis = dis(gen);

        auto comparator = (axis == 0) ? compare_box_min_x
            : (axis == 1) ? compare_box_min_y
            : compare_box_min_z;
        auto shape_span = end - start;
        if (shape_span == 1) {
            left = right = shapes_copy.at(start);
        } else if (shape_span == 2) {
            if (comparator(shapes_copy.at(start), shapes_copy.at(start+1))) {
                left = shapes_copy.at(start);
                right = shapes_copy.at(start+1);
            } else {
                left = shapes_copy.at(start+1);
                right = shapes_copy.at(start);
            }
        } else {
            std::sort(shapes_copy.begin() + start, shapes_copy.begin() + end, comparator);

            auto mid = start + (shape_span / 2);
            left = std::make_shared<BVHNode>(shapes_copy, start, mid);
            right = std::make_shared<BVHNode>(shapes_copy, mid, end);
        }


        box = BoundingBox(left->bounding_box(), right->bounding_box());
    }

    BoundingBox bounding_box() const override
    {
        return box;
    }

    bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const override
    {
        if (!box.hit(r, ray_interval))
        {
            return false;
        }
        bool hit_left = left->hit(r, ray_interval, rec);
        bool hit_right = right->hit(r, Interval(ray_interval.start(), hit_left ? rec.t : ray_interval.end()), rec);

        return hit_left || hit_right;

    }

    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    BoundingBox box;

    static bool compare_box_min(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
    {
        BoundingBox box_a = a->bounding_box();
        BoundingBox box_b = b->bounding_box();
        return a->bounding_box()[axis].start() < b->bounding_box()[axis].start();
    }

    static bool compare_box_min_x(std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b)
    {
        return compare_box_min(a, b, 0);
    }

    static bool compare_box_min_y(std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b)
    {
        return compare_box_min(a, b, 1);
    }

    static bool compare_box_min_z(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return compare_box_min(a, b, 2);
    }


};