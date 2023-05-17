#pragma once

struct config
{
    const char* shader_vertex;
    const char* shader_fragment;
    const char* texture_cells;
    const char* texture_chessboard;
    const char* texture_checker;
    double      width         = 960;
    double      height        = 540;
    bool        is_full_sreen = false;

    float camera_speed_rotate = 1. / 100.;
    float camera_speed        = 0.05;
};