#pragma once

struct config
{
    const char* shader_vertex;
    const char* shader_fragment_chessboard;
    const char* shader_fragment_chessboard_cells;
    const char* shader_fragment_checker;
    const char* texture_cells;
    double      width         = 960;
    double      height        = 540;
    bool        is_full_sreen = false;

    float camera_speed_rotate = 1. / 100.;
    float camera_speed        = 0.05;
};