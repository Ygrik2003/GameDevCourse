#pragma once

#include <cstdint>

struct event
{
    struct event_mouse
    {
        uint8_t left : 1;
        uint8_t right : 1;
        uint8_t center : 1;
        uint8_t back : 1;
        uint8_t next : 1;
    } mouse;
    struct event_keyboard
    {
        uint8_t up_clicked : 1;
        uint8_t up_released : 1;
        uint8_t down_clicked : 1;
        uint8_t down_released : 1;
        uint8_t left_clicked : 1;
        uint8_t left_released : 1;
        uint8_t right_clicked : 1;
        uint8_t right_released : 1;
    } keyboard;

    struct event_action
    {
        uint8_t quit : 1;
    } action;

    void clear()
    {
        mouse    = event_mouse{};
        keyboard = event_keyboard{};
        action   = event_action{};
    }
};
