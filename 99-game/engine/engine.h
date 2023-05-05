#include "core/config.h"
#include "core/event.h"

#include <SDL3/SDL.h>

class engine
{
public:
    virtual ~engine() = default;

    virtual int  initialize(config _config) = 0;
    virtual int  uninitialize()             = 0;
    virtual void event_keyboard()           = 0;
    // TODO

private:
    config _config;
};