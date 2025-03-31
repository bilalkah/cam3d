/**
 * @file cohen_sutherland_algorithm.hpp
 * @author https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
 * @brief
 * @version 0.1
 * @date 2025-03-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef COHEN_SUTHERLAND_ALGORITHM_H
#define COHEN_SUTHERLAND_ALGORITHM_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace cam3d
{

class Bresenham
{
  public:
    auto CalculateLine(float &x0, float &y0, float &x1, float &y1) -> std::vector<std::pair<uint32_t, uint32_t>>;
};

class CohenSutherland
{
  public:
    CohenSutherland(size_t width, size_t height);
    ~CohenSutherland() = default;

  public:
    auto CohenSutherlandLineClip(float &x0, float &y0, float &x1, float &y1) -> bool;

  private:
    size_t width_minus_1_;
    size_t height_minus_1_;

    typedef int OutCode;

    const int INSIDE = 0b0000;
    const int LEFT = 0b0001;
    const int RIGHT = 0b0010;
    const int BOTTOM = 0b0100;
    const int TOP = 0b1000;

    auto ComputeOutCode(float x, float y) -> OutCode;
};

} // namespace cam3d

#endif // COHEN_SUTHERLAND_ALGORITHM_H
