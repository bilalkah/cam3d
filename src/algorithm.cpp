#include <algorithm.hpp>
#include <cstdint>
#include <cstdlib>

namespace cam3d
{

/// @note Bresenham algorithm
/// ------------------------------------------------------------------------------  ///

auto Bresenham::CalculateLine(float &x0, float &y0, float &x1, float &y1) -> std::vector<std::pair<uint32_t, uint32_t>>
{
    std::vector<std::pair<uint32_t, uint32_t>> points;
    int dx = static_cast<int>(x1 - x0);
    int dy = static_cast<int>(y1 - y0);

    int sx = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    int sy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
    dx = std::abs(dx);
    dy = std::abs(dy);

    // Cases
    // dx != 0 && dy != 0
    if (dx != 0 && dy != 0)
    {
        if (dx > dy)
        {
            int err = dx / 2;
            while (static_cast<int>(x0) != static_cast<int>(x1))
            {
                points.emplace_back(static_cast<uint32_t>(x0), static_cast<uint32_t>(y0));
                err -= dy;
                if (err < 0)
                {
                    y0 += sy;
                    err += dx;
                }
                x0 += sx;
            }
        }
        else
        {
            int err = dy / 2;
            while (static_cast<int>(y0) != static_cast<int>(y1))
            {
                points.emplace_back(static_cast<uint32_t>(x0), static_cast<uint32_t>(y0));
                err -= dx;
                if (err < 0)
                {
                    x0 += sx;
                    err += dy;
                }
                y0 += sy;
            }
        }
    }
    else
    {
        // dx == 0 || dy == 0
        if (dx == 0)
        {
            while (static_cast<int>(y0) != static_cast<int>(y1))
            {
                points.emplace_back(static_cast<uint32_t>(x0), static_cast<uint32_t>(y0));
                y0 += sy;
            }
        }
        else
        {
            while (static_cast<int>(x0) != static_cast<int>(x1))
            {
                points.emplace_back(static_cast<uint32_t>(x0), static_cast<uint32_t>(y0));
                x0 += sx;
            }
        }
    }
    return points;
}

/// @note Cohen–Sutherland clipping algorithm
/// ------------------------------------------------------------------------------  ///

CohenSutherland::CohenSutherland(size_t width, size_t height) : width_minus_1_(width - 1), height_minus_1_(height - 1)
{
    // Constructor
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with
// diagonal from (0, 0) to (width, height)
auto CohenSutherland::CohenSutherlandLineClip(float &x0, float &y0, float &x1, float &y1) -> bool
{
    // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
    OutCode outcode0 = ComputeOutCode(x0, y0);
    OutCode outcode1 = ComputeOutCode(x1, y1);
    bool accept = false;

    while (true)
    {
        if (!(outcode0 | outcode1))
        {
            // bitwise OR is 0: both points inside window; trivially accept and exit loop
            accept = true;
            break;
        }
        else if (outcode0 & outcode1)
        {
            // bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
            // or BOTTOM), so both must be outside window; exit loop (accept is false)
            break;
        }
        else
        {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            double x, y;

            // At least one endpoint is outside the clip rectangle; pick it.
            OutCode outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

            // Now find the intersection point;
            // use formulas:
            //   slope = (y1 - y0) / (x1 - x0)
            //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
            //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
            // No need to worry about divide-by-zero because, in each case, the
            // outcode bit being tested guarantees the denominator is non-zero
            if (outcodeOut & TOP)
            { // point is above the clip window
                x = x0 + (x1 - x0) * (height_minus_1_ - y0) / (y1 - y0);
                y = height_minus_1_;
            }
            else if (outcodeOut & BOTTOM)
            { // point is below the clip window
                x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
                y = 0;
            }
            else if (outcodeOut & RIGHT)
            { // point is to the right of clip window
                y = y0 + (y1 - y0) * (width_minus_1_ - x0) / (x1 - x0);
                x = width_minus_1_;
            }
            else if (outcodeOut & LEFT)
            { // point is to the left of clip window
                y = y0 + (y1 - y0) * (0 - x0) / (x1 - x0);
                x = 0;
            }

            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcodeOut == outcode0)
            {
                x0 = x;
                y0 = y;
                outcode0 = ComputeOutCode(x0, y0);
            }
            else
            {
                x1 = x;
                y1 = y;
                outcode1 = ComputeOutCode(x1, y1);
            }
        }
    }
    return accept;
}

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)

// ASSUME THAT xmax, xmin, ymax and ymin are global constants.

auto CohenSutherland::ComputeOutCode(float x, float y) -> OutCode
{
    OutCode code = INSIDE; // initialised as being inside of clip window

    if (x < 0) // to the left of clip window
        code |= LEFT;
    else if (x > width_minus_1_) // to the right of clip window
        code |= RIGHT;
    if (y < 0) // below the clip window
        code |= BOTTOM;
    else if (y > height_minus_1_) // above the clip window
        code |= TOP;

    return code;
}

} // namespace cam3d
