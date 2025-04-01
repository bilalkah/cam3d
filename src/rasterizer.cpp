#include "vector3.hpp"
#include <algorithm>
#include <iostream>
#include <rasterizer.hpp>
namespace cam3d
{

Rasterizer::Rasterizer(uint32_t width, uint32_t height)
    : width_(width), height_(height), aspect_ratio_(static_cast<float>(width) / height), fov_(60),
      focal_length_(1 / std::tan(fov_ / 2)), near_plane_(0.1f), far_plane_(1000.0f)
{
    assert(width > 0 && height > 0 && "Width and height must be greater than zero");

    // Initialize the projection matrix
    projection_matrix_ =
        std::array<std::array<float, 4>, 4>{{{focal_length_ / aspect_ratio_, 0, 0, 0},
                                             {0, focal_length_, 0, 0},
                                             {0, 0, (far_plane_ + near_plane_) / (near_plane_ - far_plane_),
                                              (2 * far_plane_ * near_plane_) / (near_plane_ - far_plane_)},
                                             {0, 0, -1, 0}}};

    clipper_ = std::make_unique<CohenSutherland>(width, height);
    bresenham_ = std::make_unique<Bresenham>();
    intersectionCalculator_ = std::make_unique<IntersectionCalculator>();
}

auto Rasterizer::drawLine(const Vector3<float> &p_start, const Vector3<float> &p_end, FrameBuffer &fb,
                          const ARGB &color) -> void
{

    auto start = (p_start);
    auto end = p_end;

    // Clip the line
    if (!clipper_->CohenSutherlandLineClip(start.x(), start.y(), end.x(), end.y()))
    {
        return; // Line is completely outside the clipping rectangle
    }

    // Bresenham's line algorithm
    auto points = bresenham_->CalculateLine(start.x(), start.y(), end.x(), end.y());
    for (const auto &point : points)
    {
        fb.setPixel(point.first, point.second, p_start.z(), color);
    }
};

auto Rasterizer::drawTriangle(const Vector3<float> &p1, const Vector3<float> &p2, const Vector3<float> &p3,
                              FrameBuffer &fb, const ARGB &color) -> void
{
    // Draw the triangle edges
    drawLine(p1, p2, fb, color);
    drawLine(p2, p3, fb, color);
    drawLine(p3, p1, fb, color);
    // Fill the triangle using a simple scanline algorithm
    // This is a basic implementation and can be improved for performance
    // and accuracy
    auto minY = std::min({p1.y(), p2.y(), p3.y()});
    auto maxY = std::max({p1.y(), p2.y(), p3.y()});
    for (int y = static_cast<int>(minY); y <= static_cast<int>(maxY); ++y)
    {
        uint32_t x_min = width_;
        uint32_t x_max = 0;
        uint32_t z_min = std::numeric_limits<uint32_t>::max();
        // Find the intersection points with the triangle edges
        auto p1p2 = intersectionCalculator_->calculate2dSegmentIntersection(
            p1, p2, Vector3<float>(0, static_cast<float>(y), 0), Vector3<float>(width_, static_cast<float>(y), 0));
        auto p2p3 = intersectionCalculator_->calculate2dSegmentIntersection(
            p2, p3, Vector3<float>(0, static_cast<float>(y), 0), Vector3<float>(width_, static_cast<float>(y), 0));
        auto p3p1 = intersectionCalculator_->calculate2dSegmentIntersection(
            p3, p1, Vector3<float>(0, static_cast<float>(y), 0), Vector3<float>(width_, static_cast<float>(y), 0));

        // Check if the intersection points are valid
        if (p1p2.first)
        {
            x_min = std::min(x_min, static_cast<uint32_t>(p1p2.second.x()));
            x_max = std::max(x_max, static_cast<uint32_t>(p1p2.second.x()));
            z_min = std::min(z_min, static_cast<uint32_t>(p1p2.second.z()));
        }
        if (p2p3.first)
        {
            x_min = std::min(x_min, static_cast<uint32_t>(p2p3.second.x()));
            x_max = std::max(x_max, static_cast<uint32_t>(p2p3.second.x()));
            z_min = std::min(z_min, static_cast<uint32_t>(p2p3.second.z()));
        }
        if (p3p1.first)
        {
            x_min = std::min(x_min, static_cast<uint32_t>(p3p1.second.x()));
            x_max = std::max(x_max, static_cast<uint32_t>(p3p1.second.x()));
            z_min = std::min(z_min, static_cast<uint32_t>(p3p1.second.z()));
        }
        // Draw the horizontal line between the intersection points
        if (x_min < x_max)
        {
            for (uint32_t x = x_min; x <= x_max; ++x)
            {
                fb.setPixel(x, y, z_min, color);
            }
        }
    }
};

} // namespace cam3d
