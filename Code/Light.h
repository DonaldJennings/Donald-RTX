
#include "GeoVec.h"

class Light {
public:
    GeoVec position() const { return position_; }
    GeoVec intensity() const { return intensity_; }

    Light(const GeoVec& position, const GeoVec& intensity)
        : position_(position), intensity_(intensity) {}

private:
    GeoVec position_;
    GeoVec intensity_;
};
