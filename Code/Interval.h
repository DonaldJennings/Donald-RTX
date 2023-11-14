#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>

class Interval {
public:

  Interval() : min_(-std::numeric_limits<double>::infinity()), max_(std::numeric_limits<double>::infinity()) {} 
  Interval(double start, double end) : min_(start), max_(end) {}
  Interval(const Interval& a, const Interval& b)
      : min_(fmin(a.min_, b.min_)), max_(fmax(a.max_, b.max_)) {}

  double start() const { return min_; }
  double end() const { return max_; }
  double length() const { return max_ - min_; }
  bool contains(double value) const { return value >= min_ && value <= max_; }
  bool surrounds(double value) const { return min_ < value && value < max_; }

  double clamp(double value) const {
    if (value < min_) {
      return min_;
    } else if (value > max_) {
      return max_;
    } else {
      return value;
    }
  }

  double min_;
  double max_;
};


inline std::ostream& operator<<(std::ostream& os, const Interval& i) {
  os << "[" << i.start() << ", " << i.end() << "]";
  return os;
}