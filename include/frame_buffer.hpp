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

#include <X11/X.h>
#include <cassert>
#include <cstdint>
#include <limits>
#include <numeric>
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

struct COLOR
{
    ARGB RED{255, 255, 0, 0};
    ARGB GREEN{255, 0, 255, 0};
    ARGB BLUE{255, 0, 0, 255};
    ARGB WHITE{255, 255, 255, 255};
    ARGB BLACK{255, 0, 0, 0};
    ARGB YELLOW{255, 255, 255, 0};
    ARGB CYAN{255, 0, 255, 255};
    ARGB MAGENTA{255, 255, 0, 255};
    ARGB GRAY{255, 128, 128, 128};
};

class FrameBuffer
{
  public:
    FrameBuffer(uint32_t width, uint32_t height)
        : width_(width), height_(height), total_size_(width * height), buffer_(total_size_, ARGB()),
          depth_buffer_(total_size_, std::numeric_limits<float>::max())
    {
        assert(width > 0 && height > 0 && "Width and height must be greater than zero");
    }
    ~FrameBuffer() = default;

    auto clear() -> void
    {
        std::fill(buffer_.begin(), buffer_.end(), ARGB());
        std::fill(depth_buffer_.begin(), depth_buffer_.end(), std::numeric_limits<uint32_t>::max());
    }

    auto clear(const ARGB &color) -> void
    {
        std::fill(buffer_.begin(), buffer_.end(), color);
        std::fill(depth_buffer_.begin(), depth_buffer_.end(), std::numeric_limits<uint32_t>::max());
    }

    auto setPixel(uint32_t x, uint32_t y, const ARGB &pixel) -> void
    {
        assert(x < width_ && y < height_ && "Pixel coordinates out of bounds");
        size_t index = y * width_ + x;
        buffer_[index] = pixel;
    }

    auto setPixel(uint32_t x, uint32_t y, uint32_t z, const ARGB &pixel) -> void
    {
        assert(x < width_ && y < height_ && "Pixel coordinates out of bounds");
        size_t index = y * width_ + x;
        if (z < depth_buffer_[index])
        {
            buffer_[index] = pixel;
            depth_buffer_[index] = z;
        }
    }

    auto getDepth(uint32_t x, uint32_t y) const -> uint32_t
    {
        assert(x < width_ && y < height_ && "Pixel coordinates out of bounds");
        size_t index = y * width_ + x;
        return depth_buffer_[index];
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

    auto getBuffer() const -> const BufferARGB &
    {
        return buffer_;
    }

    auto getDepthBuffer() -> std::vector<float> &
    {
        return depth_buffer_;
    }

    auto getDepthBuffer() const -> const std::vector<float> &
    {
        return depth_buffer_;
    }

  private:
    uint32_t width_;
    uint32_t height_;
    size_t total_size_;
    BufferARGB buffer_;
    std::vector<float> depth_buffer_;
}; // FrameBuffer class definition

} // namespace cam3d

#endif // FRAME_BUFFER_H
