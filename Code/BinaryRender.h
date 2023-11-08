#pragma once
#include "RenderMode.h"
#include "GeoVec.h"
#include "Ray.h"
#include "World.h"

class BinaryRender : public RenderMode {
public:
  BinaryRender() {};

  virtual GeoVec compute_colour(Ray& ray, int depth, World& world) const override
   {
    // Find the closest intersection point
    HitRecord hitRecord;
    if (!world.hit(ray, Interval(0.001, INFINITY), hitRecord))
    {
      return GeoVec(0, 0, 0); // Return black if no intersection
    }

    // Return red if there is an intersection
    return GeoVec(1,0,0);
  }
};
