#pragma once
class Interval {
public:
  Interval(double start, double end) : start_(start), end_(end) {}

  double start() const { return start_; }
  double end() const { return end_; }
  double length() const { return end_ - start_; }
  bool contains(double value) const { return value >= start_ && value <= end_; }
  bool surrounds(double value) const { return start_ < value && value < end_; }
  double clamp(double x) const {
    if (x < start_) return start_;
    if (x > end_) return end_;
    return x;
  }
  
private:
  double start_;
  double end_;
};