#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "GeoVec.h"

class Texture {
public:
    Texture() {}
    Texture(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open texture file");
        }

        std::string format;
        file >> format;
        if (format != "P3") {
            throw std::runtime_error("Unsupported texture format");
        }

        file >> m_width >> m_height >> m_maxValue;
        m_data.resize(m_width * m_height);

        for (int i = 0; i < m_width * m_height; i++) {
            int r, g, b;
            file >> r >> g >> b;
            m_data[i] = GeoVec(r / 255.0, g / 255.0, b / 255.0);
        }
    }

    GeoVec sample(double& u, double& v) const
    {
        // Ensure u and v are in the range [0, 1]
        u = fmod(u, 1.0);
        if (u < 0) u += 1.0;
        v = fmod(v, 1.0);
        if (v < 0) v += 1.0;

        // Compute the texture coordinates
        int x = static_cast<int>(u * m_width);
        int y = static_cast<int>(v * m_height);

        // Ensure x and y are within the texture dimensions
        x = std::min(std::max(x, 0), m_width - 1);
        y = std::min(std::max(y, 0), m_height - 1);

        // Sample the texture
        return m_data.at(y * m_width + x);
    }


private:
    int m_width;
    int m_height;
    int m_maxValue;
    std::vector<GeoVec> m_data;
};
