#pragma once
#include <cmath>

class GeoVec {
public:
    float x, y, z;

    GeoVec() : x(0), y(0), z(0) {}
    GeoVec(float x, float y, float z) : x(x), y(y), z(z) {}

    // Unary operators
    GeoVec operator-() const {
        return GeoVec(-x, -y, -z);
    }

    // Binary operators
    GeoVec operator+(const GeoVec& other) const {
        return GeoVec(x + other.x, y + other.y, z + other.z);
    }

	GeoVec operator+(const float& other) const
	{
		return GeoVec(x + other, y + other, z + other);
	}

    GeoVec operator-(const GeoVec& other) const {
        return GeoVec(x - other.x, y - other.y, z - other.z);
    }

    GeoVec operator*(float scalar) const {
        return GeoVec(x * scalar, y * scalar, z * scalar);
    }

    GeoVec operator/(float scalar) const {
        return GeoVec(x / scalar, y / scalar, z / scalar);
    }

    // Compound assignment operators
    GeoVec& operator+=(const GeoVec& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    GeoVec& operator-=(const GeoVec& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    GeoVec& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    GeoVec& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Comparison operators
    bool operator==(const GeoVec& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const GeoVec& other) const {
        return !(*this == other);
    }

    // Other methods
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

	GeoVec normalize() const {
		return *this / length();
	}
};

// dot product
float dot(const GeoVec& v1, const GeoVec& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// cross product
GeoVec cross(const GeoVec& v1, const GeoVec& v2) {
	return GeoVec(
		v1.y * v2.z - v1.z * v2.y,
		-(v1.x * v2.z - v1.z * v2.x),
		v1.x * v2.y - v1.y * v2.x
	);
}

GeoVec operator*(float scalar, const GeoVec& v) {
	return GeoVec(v.x * scalar, v.y * scalar, v.z * scalar);
}