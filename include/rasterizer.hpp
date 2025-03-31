/**
 * @file rasterizer.hpp
 * @author Bilal Kahraman (kahramannbilal@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <algorithm.hpp>
#include <cstddef>
#include <frame_buffer.hpp>
#include <memory>
#include <vector3.hpp>

namespace cam3d
{

class Rasterizer
{
  public:
    Rasterizer(size_t width, size_t height);

    ~Rasterizer() = default;

    auto drawLine(const Vector3<float> &p_start, const Vector3<float> &p_end, FrameBuffer &fb,
                  const ARGB &color) -> void;

    auto drawTriangle(const Vector3<float> &p1, const Vector3<float> &p2, const Vector3<float> &p3, FrameBuffer &fb,
                      const ARGB &color) -> void;

  private:
    size_t width_;
    size_t height_;

    std::unique_ptr<CohenSutherland> clipper_;
    std::unique_ptr<Bresenham> bresenham_;
    std::unique_ptr<IntersectionCalculator> intersectionCalculator_;
};

} // namespace cam3d
#endif // RASTERIZER_H
