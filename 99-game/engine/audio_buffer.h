#include "SDL3/SDL.h"
#include <memory>
#include <mutex>

#pragma pack(push, 1)
class audio_buffer
{
public:
    enum class state
    {
        once,
        looped
    };

    audio_buffer(const char* path, uint32_t device, SDL_AudioSpec audio_spec);
    ~audio_buffer();

    void play(const state st) final
    {
        std::lock_guard<std::mutex> lock(engine::audio_mutex);
        // here we can change properties
        // of sound and dont collade with multithreaded playing
        current_index = 0;
        is_playing    = true;
        is_looped     = (st == state::looped);
    }

    std::unique_ptr<uint8_t[]> tmp_buf;
    uint8_t*                   buffer;
    uint32_t                   length;
    uint32_t                   current_index = 0;
    SDL_AudioDeviceID          device;
    bool                       is_playing = false;
    bool                       is_looped  = false;
};
#pragma pack(pop)
