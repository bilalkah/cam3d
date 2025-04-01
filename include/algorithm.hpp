/**
 * @file cohen_sutherland_algorithm.hpp
 * @author Bilal Kahraman (kahramannbilal@gmail.com)
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
#include <vector3.hpp>
#include <vector>

namespace cam3d
{

class IntersectionCalculator
{
  public:
    auto calculate2dSegmentIntersection(const Vector3<float> &l1_start, const Vector3<float> &l1_end,
                                        const Vector3<float> &l2_start,
                                        const Vector3<float> &l2_end) -> std::pair<bool, Vector3<float>>;
    auto calculate2dLineIntersection(const Vector3<float> &l1_start, const Vector3<float> &l1_end,
                                     const Vector3<float> &l2_start,
                                     const Vector3<float> &l2_end) -> std::pair<bool, Vector3<float>>;
};

// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
class Bresenham
{
  public:
    auto CalculateLine(float &x0, float &y0, float &x1, float &y1) -> std::vector<std::pair<uint32_t, uint32_t>>;
};

// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
class CohenSutherland
{
  public:
    CohenSutherland(uint32_t width, uint32_t height);
    ~CohenSutherland() = default;

  public:
    auto CohenSutherlandLineClip(float &x0, float &y0, float &x1, float &y1) -> bool;

  private:
    uint32_t width_minus_1_;
    uint32_t height_minus_1_;

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
