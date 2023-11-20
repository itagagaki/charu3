#pragma once

#include "stdafx.h"

#include <string>

namespace Color {

    uint32_t Parse(const std::string str);
    std::string String(uint32_t color);

    COLORREF Swap_RGB_BGR(COLORREF color);
    COLORREF MultiplyBrightness(COLORREF color, double brightness);

} // namespace Color
