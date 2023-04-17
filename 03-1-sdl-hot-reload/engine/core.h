#pragma once

struct point
{
    point();
    point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    int x = 0;
    int y = 0;
};

#pragma pack(push, 1)
struct config
{
    config(uint8_t max_chars,
           char    char_placeholder,
           char    char_start,
           char    char_end,
           uint8_t speed)
    {
        this->max_chars        = max_chars;
        this->char_placeholder = char_placeholder;
        this->char_start       = char_start;
        this->char_end         = char_end;
        this->speed            = speed;
    }
    uint8_t max_chars        = 5;
    char    char_placeholder = '#';
    char    char_start       = '[';
    char    char_end         = ']';
    uint8_t speed            = 2; // speed typing chars per second
};
#pragma pack(pop)