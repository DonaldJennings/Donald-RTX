#pragma once
class Interval {
public:
  Interval(double start, double end) : min(start), max(end) {}

  double start() const { return min; }
  double end() const { return max; }
  double length() const { return max - min; }
  bool contains(double value) const { return value >= min && value <= max; }
  bool surrounds(double value) const { return min < value && value < max; }

  double min;
  double max;
};