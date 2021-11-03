#pragma once

#include <cmath>
#include <array>
#include <ostream>

using distance_t = double;

template <typename DataType, std::size_t DIM>
class Point {
 private:
  char key;
  std::array<DataType, DIM> _values;

 public:
  Point() { std::fill_n(std::begin(_values), DIM, DataType(0)); }

  Point(const std::array<DataType, DIM>& init_array) : _values(init_array) {}

  Point(char key, const std::array<DataType, DIM>& init_array) : _values(init_array) 
  {
    this->key = key;
  }

  bool operator==(const Point<DataType, DIM>& p) const;

  bool operator<(const Point<DataType, DIM>& p) const;

  bool operator<=(const Point<DataType, DIM>& p) const;

  distance_t distance(const Point<DataType, DIM>& p) const;
  distance_t getArea(const Point<DataType, DIM>& p) const;

  DataType get(std::size_t pos) const { return _values.at(pos); }
  void set(std::size_t pos, DataType value) { _values[pos] = value; }

  char getKey() { return key; }

  std::size_t size() { return DIM; }
};

template <typename DataType, std::size_t DIM>
inline bool Point<DataType, DIM>::operator==(
    const Point<DataType, DIM>& p) const {
  for (std::size_t I = 0; I < DIM; I++) {
    if (this->_values.at(I) != p._values.at(I)) return false;
  }
  return true;
}

template <typename DataType, std::size_t DIM>
inline bool Point<DataType, DIM>::operator<(
    const Point<DataType, DIM>& p) const {
  for (std::size_t I = 0; I < DIM; I++) {
    if (this->_values.at(I) >= p._values.at(I)) return false;
  }
  return true;
}

template <typename DataType, std::size_t DIM>
inline bool Point<DataType, DIM>::operator<=(
    const Point<DataType, DIM>& p) const {
  for (std::size_t I = 0; I < DIM; I++) {
    if (this->_values.at(I) > p._values.at(I)) return false;
  }
  return true;
}

template <typename DataType, std::size_t DIM>
std::ostream& operator<<(std::ostream& os, Point<DataType, DIM>& obj) {
  return os << obj.getKey(); // <<  ": { " << obj.get(0) << ", " << obj.get(1) << " }";
}

template <typename DataType, std::size_t DIM>
inline distance_t Point<DataType, DIM>::getArea(
    const Point<DataType, DIM>& p) const {
    return abs(this->_values.at(0) - p.get(0)) * abs(this->_values.at(1) - p.get(1));
}

template <typename DataType, std::size_t DIM>
inline distance_t Point<DataType, DIM>::distance(
    const Point<DataType, DIM>& p) const {
  double acum = 0.0;

  for (std::size_t I = 0; I < DIM; I++) {
    acum += (this->_values.at(I) - p._values.at(I)) *
            (this->_values.at(I) - p._values.at(I));
  }
  return sqrt(acum);
}

using point_t = Point<distance_t, 2>;