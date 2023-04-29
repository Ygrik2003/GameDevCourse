#pragma once

#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <vector>

constexpr uint8_t color_format = 255;

#pragma pack(push, 1)
struct rgb
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    friend bool operator==(const rgb& l, const rgb& r);

    friend std::ostream& operator<<(std::ostream& out, const rgb& color);

    static rgb generate();
};
#pragma pack(pop)

constexpr rgb color_red{ 255, 0, 0 };
constexpr rgb color_green{ 0, 255, 0 };
constexpr rgb color_blue{ 0, 0, 255 };

class canvas
{
public:
    canvas(size_t w, size_t h);

    void save_image(const char* path);
    void load_image(const char* path);

    void             set_pixel(size_t x, size_t y, rgb color);
    rgb              get_pixel(size_t x, size_t y) const;
    std::vector<rgb> get_pixels() const;

    auto begin() { return pixels.begin(); }
    auto end() { return pixels.end(); }

    bool operator==(const canvas& other) const;
    bool operator!=(const canvas& other) const;

private:
    size_t           width  = 0;
    size_t           height = 0;
    std::vector<rgb> pixels;
};
