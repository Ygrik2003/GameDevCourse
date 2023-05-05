#include "core/config.h"
#include "core/event.h"

class game
{
public:
    virtual ~game()                    = default;
    virtual int  initialize(config)    = 0;
    virtual void event_listener(event) = 0;
    virtual void update()              = 0;
    virtual void render() const        = 0;
};