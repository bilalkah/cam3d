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
#include <array>
#include <cstddef>
#include <frame_buffer.hpp>
#include <memory>
#include <vector3.hpp>

namespace cam3d
{

class Rasterizer
{
  public:
    Rasterizer(uint32_t width, uint32_t height);

    ~Rasterizer() = default;

    auto drawLine(const Vector3<float> &p_start, const Vector3<float> &p_end, FrameBuffer &fb,
                  const ARGB &color) -> void;

    auto drawTriangle(const Vector3<float> &p1, const Vector3<float> &p2, const Vector3<float> &p3, FrameBuffer &fb,
                      const ARGB &color) -> void;

    template <typename T> auto normalizeToScreen(const Vector3<T> &v) -> Vector3<T>;
    template <typename T> auto projectOrtographic(const Vector3<T> &v) -> Vector3<T>;
    template <typename T> auto projectBasicPerspective(const Vector3<T> &v) -> Vector3<T>;

  private:
    uint32_t width_;
    uint32_t height_;
    float aspect_ratio_;
    float fov_;
    float focal_length_;
    float near_plane_;
    float far_plane_;
    std::array<std::array<float, 4>, 4> projection_matrix_;

    std::unique_ptr<CohenSutherland> clipper_;
    std::unique_ptr<Bresenham> bresenham_;
    std::unique_ptr<IntersectionCalculator> intersectionCalculator_;
};

/**
 * @brief Normalizes a 3D vector to screen coordinates
 *
 * @tparam T The type of the vector components (e.g., float, double)
 * @param v The 3D vector to normalize.
 * @param s_width The width of the screen
 * @param s_height The height of the screen
 * @return Vector3<T> The normalized 2D vector between [-1, 1] for x and y.
 * @note The z component will be kept as before.
 */
template <typename T> auto Rasterizer::normalizeToScreen(const Vector3<T> &v) -> Vector3<T>
{
    T x = (v.x() / width_) * 2 - 1;  // Normalize to [-1, 1]
    T y = 1 - (v.y() / height_) * 2; // Normalize to [-1, 1]
    return Vector3<T>(x, y, v.z());
}

/**
 * @brief Projects a 3D vector to 2D using orthographic projection
 *
 * @tparam T The type of the vector components (e.g., float, double)
 * @param v The 3D vector to project. Range [-1, 1] for x and y.
 * @param s_width The width of the screen
 * @param s_height The height of the screen
 * @return Vector3<T> The projected 2D vector
 */
template <typename T> auto Rasterizer::projectOrtographic(const Vector3<T> &v) -> Vector3<T>
{
    T x = ((v.x() + 1) / 2) * (width_ - 1);
    T y = ((1 - v.y()) / 2) * (height_ - 1);
    return Vector3<T>(x, y, v.z());
};

/**
 * @brief Projects a 3D vector to 2D using perspective projection
 *
 * @tparam T The type of the vector components (e.g., float, double)
 * @param v The 3D vector to project. Range [-1, 1] for x and y.
 * @param s_width The width of the screen
 * @param s_height The height of the screen
 * @return Vector3<T> The projected 2D vector
 */
template <typename T> auto Rasterizer::projectBasicPerspective(const Vector3<T> &v) -> Vector3<T>
{
    if (v.z() < near_plane_ || v.z() > far_plane_)
    {
        return Vector3<T>(0, 0, 0); // Point is outside the view frustum
    }

    std::array<float, 4> h_coord = {v.x(), v.y(), v.z(), 1.0f};
    std::array<float, 4> projected_coord = {0.0f, 0.0f, 0.0f, 0.0f};
    projected_coord[0] = projection_matrix_[0][0] * h_coord[0];
    projected_coord[1] = projection_matrix_[1][1] * h_coord[1];
    projected_coord[2] = projection_matrix_[2][2] * h_coord[2] + projection_matrix_[2][3];
    projected_coord[3] = projection_matrix_[3][2] * h_coord[2];

    // Perspective divide
    projected_coord[0] /= projected_coord[3];
    projected_coord[1] /= projected_coord[3];
    projected_coord[2] /= projected_coord[3];

    // Normalize to screen coordinates
    T x = ((projected_coord[0] + 1) / 2) * (width_ - 1);
    T y = ((1 - projected_coord[1]) / 2) * (height_ - 1);
    T z = (v.z() - near_plane_) / (far_plane_ - near_plane_); // Normalize z to [0, 1]
    return Vector3<T>(x, y, z);
}

} // namespace cam3d
#endif // RASTERIZER_H
