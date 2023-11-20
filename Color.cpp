#include "stdafx.h"

#include <string>
#include <regex>
#include <vector>

#include "Color.h"

uint32_t Color::Parse(const std::string str)
{
    std::smatch m;
    if (std::regex_match(str, m, std::regex("^\\s*#([0-9a-fA-F]{3})\\s*$"))) {
        long rgb = std::stol(m[1].str(), nullptr, 16);
        long r = (rgb >> 8) & 0x0f;
        long g = (rgb >> 4) & 0x0f;
        long b = rgb & 0x0f;
        return static_cast<uint32_t>((((r << 4) | r) << 16) | (((g << 4) | g) << 8) | ((b << 4) | b));
    }
    else if (std::regex_match(str, m, std::regex("^\\s*#([0-9a-fA-F]{6})\\s*$"))) {
        return static_cast<uint32_t>(std::stol(m[1].str(), nullptr, 16));
    }
    return 0;
}

std::string Color::String(uint32_t color)
{
    const size_t len = 7; // #rrggbb
    const size_t size = len + 1; // +1 for terminater (nul)
    std::vector<char> buf(size);
    std::snprintf(&buf[0], size, "#%.6x", color);
    return std::string(&buf[0], &buf[0] + len);
}

COLORREF Color::Swap_RGB_BGR(COLORREF color)
{
    COLORREF HighToLow = (0x00ff0000 & color) >> 16;
    COLORREF G = 0x0000ff00 & color;
    COLORREF LowToHigh = (0x000000ff & color) << 16;
    return LowToHigh | G | HighToLow;
}

COLORREF Color::MultiplyBrightness(COLORREF color, double brightness)
{
    COLORREF R, G, B;

    R = (0x00ff0000 & color) >> 16;
    R = (COLORREF)(R * brightness);
    if (R > 0xff) R = 0xff;

    G = (0x0000ff00 & color) >> 8;
    G = (COLORREF)(G * brightness);
    if (G > 0xff) G = 0xff;

    B = 0x000000ff & color;
    B = (COLORREF)(B * brightness);
    if (B > 0xff) B = 0xff;

    return (R << 16) | (G << 8) | B;
}

