#include "Color.h"

#include <string>
#include <regex>
#include <vector>

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
