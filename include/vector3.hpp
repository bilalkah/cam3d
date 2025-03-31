/**
 * @file vector3.hpp
 * @author Bilal Kahraman (kahramannbilal@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cassert>
#include <cmath>

namespace cam3d
{

/**
 * @brief A simple 3D vector class
 *
 * @tparam T The type of the vector components (e.g., float, double)
 */
template <typename T> class Vector3
{
  public:
    Vector3() : x_(0), y_(0), z_(0)
    {
    }
    Vector3(T x, T y, T z) : x_(x), y_(y), z_(z)
    {
    }

    auto length() const -> T
    {
        return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    }

    auto normalize() -> void
    {
        T len = length();
        if (len > 0)
        {
            x_ /= len;
            y_ /= len;
            z_ /= len;
        }
    }
    auto dot(const Vector3 &other) const -> T
    {
        return x_ * other.x() + y_ * other.y() + z_ * other.z();
    }
    auto cross(const Vector3 &other) const -> Vector3
    {
        return Vector3(y_ * other.z() - z_ * other.y(), z_ * other.x() - x_ * other.z(),
                       x_ * other.y() - y_ * other.x());
    }

    // Operator overloads
    template <typename U> explicit operator Vector3<U>() const
    {
        return Vector3<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
    }

    auto operator+(const Vector3 &other) const -> Vector3
    {
        return Vector3(x_ + other.x(), y_ + other.y(), z_ + other.z());
    }

    auto operator-(const Vector3 &other) const -> Vector3
    {
        return Vector3(x_ - other.x(), y_ - other.y(), z_ - other.z());
    }

    auto operator*(const T &scalar) const -> Vector3
    {
        return Vector3(x_ * scalar, y_ * scalar, z_ * scalar);
    }

    auto operator/(const T &scalar) const -> Vector3
    {
        assert(scalar != 0 && "Division by zero");
        return Vector3(x_ / scalar, y_ / scalar, z_ / scalar);
    }
    auto operator+=(const Vector3 &other) -> Vector3 &
    {
        x_ += other.x();
        y_ += other.y();
        z_ += other.z();
        return *this;
    }
    auto operator-=(const Vector3 &other) -> Vector3 &
    {
        x_ -= other.x();
        y_ -= other.y();
        z_ -= other.z();
        return *this;
    }
    auto operator*=(const T &scalar) -> Vector3 &
    {
        x_ *= scalar;
        y_ *= scalar;
        z_ *= scalar;
        return *this;
    }
    auto operator/=(const T &scalar) -> Vector3 &
    {
        assert(scalar != 0 && "Division by zero");
        x_ /= scalar;
        y_ /= scalar;
        z_ /= scalar;
        return *this;
    }
    auto operator==(const Vector3 &other) const -> bool
    {
        return (x_ == other.x() && y_ == other.y() && z_ == other.z());
    }
    auto operator!=(const Vector3 &other) const -> bool
    {
        return !(*this == other);
    }

    auto x() const -> const T &
    {
        return x_;
    }
    auto y() const -> const T &
    {
        return y_;
    }
    auto z() const -> const T &
    {
        return z_;
    }

    auto x() -> T &
    {
        return x_;
    }
    auto y() -> T &
    {
        return y_;
    }
    auto z() -> T &
    {
        return z_;
    }

  private:
    T x_, y_, z_;
};

namespace utility
{

/**
 * @brief Projects a 3D vector to 2D using orthographic projection
 *
 * @tparam T The type of the vector components (e.g., float, double)
 * @param v The 3D vector to project. Range [-1, 1] for x and y.
 * @param s_width The width of the screen
 * @param s_height The height of the screen
 * @return Vector3<T> The projected 2D vector
 */
template <typename T> auto projectOrtographic(const Vector3<T> &v, int s_width, int s_height) -> Vector3<T>
{
    T x = ((v.x() + 1) / 2) * (s_width - 1);
    T y = ((1 - v.y()) / 2) * (s_height - 1);
    return Vector3<T>(x, y, 0);
};
} // namespace utility

} // namespace cam3d

#endif // VECTOR3_H
