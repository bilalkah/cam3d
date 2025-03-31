/**
 * @file frame_buffer.h
 * @author Bilal Kahraman (kahramannbilal@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <cassert>
#include <cstdint>
#include <vector>

namespace cam3d
{

struct ARGB
{
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    ARGB() : a(0), r(0), g(0), b(0)
    {
    }
    ARGB(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue) : a(alpha), r(red), g(green), b(blue)
    {
    }

    auto toUint32() const -> uint32_t
    {
        return (static_cast<uint32_t>(a) << 24) | (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) |
               static_cast<uint32_t>(b);
    }
};

using BufferARGB = std::vector<ARGB>;

class FrameBuffer
{
  public:
    FrameBuffer(uint32_t width, uint32_t height)
        : width_(width), height_(height), total_size_(width * height), buffer_(total_size_, ARGB())
    {
        assert(width > 0 && height > 0 && "Width and height must be greater than zero");
    }
    ~FrameBuffer() = default;

    auto clear() -> void
    {
        std::fill(buffer_.begin(), buffer_.end(), ARGB());
    }

    auto clear(const ARGB &color) -> void
    {
        std::fill(buffer_.begin(), buffer_.end(), color);
    }

    auto setPixel(uint32_t x, uint32_t y, const ARGB &pixel) -> void
    {
        assert(x < width_ && y < height_ && "Pixel coordinates out of bounds");
        size_t index = y * width_ + x;
        buffer_[index] = pixel;
    }

    auto getPixel(uint32_t x, uint32_t y) const -> ARGB
    {
        assert(x < width_ && y < height_ && "Pixel coordinates out of bounds");
        size_t index = y * width_ + x;
        return buffer_[index];
    }

    auto getBuffer() -> BufferARGB &
    {
        return buffer_;
    }

  private:
    uint32_t width_;
    uint32_t height_;
    size_t total_size_;
    BufferARGB buffer_;
}; // FrameBuffer class definition

} // namespace cam3d

#endif // FRAME_BUFFER_H
