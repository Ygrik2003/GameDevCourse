#include "canvas_basic.h"

bool operator==(const rgb& l, const rgb& r)
{
    return l.r == r.r && l.g == r.g && l.b == r.b;
}

constexpr size_t color_size = sizeof(rgb);
static_assert(3 == color_size, "24 bit per pixel(r,g,b)");

std::ostream& operator<<(std::ostream& out, const rgb& color)
{
    // clang-format off
    out << "RGB("
        << static_cast<int>(color.r) << ", "
        << static_cast<int>(color.g) << ", "
        << static_cast<int>(color.b) << ")";
    // clang-format on
    return out;
}

rgb rgb::generate()
{
    // clang-format off
    return
    {
        static_cast<uint8_t>(rand() % 256),
        static_cast<uint8_t>(rand() % 256),
        static_cast<uint8_t>(rand() % 256)
    };
    // clang-format on
}

canvas::canvas(size_t w, size_t h)
{
    width  = w;
    height = h;
    pixels.resize(w * h);
}

void canvas::save_image(const char* path)
{
    std::FILE* file = std::fopen(path, "wb");
    if (!file)
        throw std::runtime_error("Can't open file");

    std::fprintf(file, "P6\n%d %d %d\n", width, height, color_format);

    std::fwrite(pixels.data(), sizeof(rgb), pixels.size(), file);
    std::fclose(file);
}

void canvas::load_image(const char* path)
{
    std::FILE* file = std::fopen(path, "rb");
    if (!file)
        throw std::runtime_error("Can't open file");

    char    header[4];
    char    temp;
    uint8_t color_format_temp;

    std::fscanf(file, "P6\n%d %d %d\n", &width, &height, &color_format_temp);

    if (color_format != color_format_temp)
        throw std::runtime_error("This PPM file doesnt fit the color format.");

    pixels.resize(width * height);

    std::fread(pixels.data(), sizeof(rgb), pixels.size(), file);
    std::fread(&temp, sizeof(char), 1, file);

    // if (!iswspace(temp))
    //     throw std::runtime_error("This PPM file is bad.");

    std::fclose(file);
}

void canvas::set_pixel(size_t x, size_t y, rgb color)
{
    if (x >= width || y >= height)
        return;

    rgb& pixel = pixels.at(y * width + x);
    pixel      = color;
}

rgb canvas::get_pixel(size_t x, size_t y) const
{
    return pixels.at(y * width + x);
}

std::vector<rgb>& canvas::get_pixels()
{
    return pixels;
}

bool canvas::operator==(const canvas& other) const
{
    return pixels == other.pixels;
}

bool canvas::operator!=(const canvas& other) const
{
    return !(*this == other);
}
