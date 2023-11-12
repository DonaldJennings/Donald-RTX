
#include "GeoVec.h"

class Light {
public:
    GeoVec position() const { return position_; }
    GeoVec intensity() const { return intensity_; }

    Light(const GeoVec& position, const GeoVec& intensity)
        : position_(position), intensity_(intensity) {}

    // << COPILOT >> Write the sample function for a point light
    GeoVec sample() const {
        // Generate a small random offset
        GeoVec offset((double)rand() / RAND_MAX - 0.5, 
                      (double)rand() / RAND_MAX - 0.5, 
                      (double)rand() / RAND_MAX - 0.5);
        offset *= 0.1;  // Adjust the magnitude of the offset as needed

        // Add the offset to the light's position
        return position_ + offset;
    }
private:
    GeoVec position_;
    GeoVec intensity_;
};
