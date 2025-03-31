#include <algorithm>
#include <rasterizer.hpp>

namespace cam3d
{

Rasterizer::Rasterizer(size_t width, size_t height) : width_(width), height_(height)
{
    assert(width > 0 && height > 0 && "Width and height must be greater than zero");
    clipper_ = std::make_unique<CohenSutherland>(width, height);
    bresenham_ = std::make_unique<Bresenham>();
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
        fb.setPixel(point.first, point.second, color);
    }
};

} // namespace cam3d