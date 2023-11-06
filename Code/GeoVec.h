#pragma once
class GeoVec
{
	public:
	GeoVec();
	GeoVec(double x, double y, double z);

	double x = 0;
	double y = 0;
	double z = 0;

	GeoVec operator+(const GeoVec& v);
	GeoVec operator-(const GeoVec& v);
	GeoVec operator*(const GeoVec& v);
	GeoVec operator/(const GeoVec& v);

	GeoVec operator+(const double& f);
	GeoVec operator-(const double& f);
	GeoVec operator*(const double& f);
	GeoVec operator/(const double& f);

	GeoVec operator+=(const GeoVec& v);
	GeoVec operator-=(const GeoVec& v);
	GeoVec operator*=(const GeoVec& v);
	GeoVec operator/=(const GeoVec& v);

	GeoVec operator+=(const double& f);
	GeoVec operator-=(const double& f);
	GeoVec operator*=(const double& f);
	GeoVec operator/=(const double& f);

	bool operator==(const GeoVec& v);
	bool operator!=(const GeoVec& v);

	double magnitude();
	GeoVec normalize();
	double dot(const GeoVec& v);
	GeoVec cross(const GeoVec& v);
};

inline GeoVec operator*(const double& f, const GeoVec& v)
{
	return GeoVec(v.x * f, v.y * f, v.z * f);
}

inline GeoVec operator/(const double& f, const GeoVec& v)
{
	return GeoVec(v.x / f, v.y / f, v.z / f);
}

inline GeoVec operator+(const double& f, const GeoVec& v)
{
	return GeoVec(v.x + f, v.y + f, v.z + f);
}


