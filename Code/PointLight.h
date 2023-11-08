
#include "GeoVec.h"

class PointLight {
public:
    PointLight(const GeoVec& position, const GeoVec& intensity)
        : position_(position), intensity_(intensity) {}

    GeoVec getPosition() const { return position_; }
    GeoVec getIntensity() const { return intensity_; }

private:
    GeoVec position_;
    GeoVec intensity_;
};
