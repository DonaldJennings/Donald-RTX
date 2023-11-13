
#include "GeoVec.h"
#include <random>

class Light {
public:
    GeoVec position() const { return position_; }
    GeoVec intensity() const { return intensity_; }

    Light(const GeoVec& position, const GeoVec& intensity)
        : position_(position), intensity_(intensity) {}

    // << COPILOT >> Write the sample function for a point light
    GeoVec sample() const {
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<double> distribution(-0.5, 0.5);

        // Generate a small random offset
        GeoVec offset(distribution(generator), 
                      distribution(generator), 
                      distribution(generator));
        offset *= 0.1;  // Adjust the magnitude of the offset as needed

        // Add the offset to the light's position
        return position_ + offset;
    }
private:
    GeoVec position_;
    GeoVec intensity_;
};
