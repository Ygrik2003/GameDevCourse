#pragma once
#include <array>
#include <iosfwd>
#include <string>

#include "core.h"

namespace yg
{

struct event
{
    struct
    {
        uint8_t left : 1;
        uint8_t right : 1;
        uint8_t center : 1;
        uint8_t back : 1;
        uint8_t next : 1;
    } mouse;
    struct
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

    void clear()
    {
        (uint8_t&)this->mouse    = 0;
        (uint8_t&)this->keyboard = 0;
    }
};

class engine
{
public:
    virtual ~engine();

    virtual int initialize(config cfg) = 0;

    virtual bool read_input()   = 0;
    virtual void uninitialize() = 0;
};

engine* create_engine();
void    destroy_engine(engine* e);

struct game
{
    virtual ~game()              = default;
    virtual void initialize()    = 0;
    virtual void on_event(event) = 0;
    virtual void update()        = 0;
    virtual void render() const  = 0;
};

} // namespace yg

extern "C" yg::game* start_game(yg::engine*);
extern "C" void      destroy_game(yg::game*);
