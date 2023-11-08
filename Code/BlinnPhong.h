
#pragma once
#include "RenderMode.h"
#include "HitRecord.h"
#include <iostream>

class BlinnPhong : public RenderMode
{
    public:
        GeoVec compute_colour(Ray& ray, int depth, World& world) const override;
};  
