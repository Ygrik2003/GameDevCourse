#include <SDL2/SDL_version.h>
#include <cstdlib>
#include <iostream>

std::ostream& operator<<(std::ostream& out, SDL_version sdl_v)
{
    out << (int)sdl_v.major << ".";
    out << (int)sdl_v.minor << ".";
    out << (int)sdl_v.patch;

    return out;
}

int main()
{
    SDL_version version_linked  = { 0, 0, 0 };
    SDL_version version_compile = { 0, 0, 0 };

    SDL_VERSION(&version_linked);
    SDL_GetVersion(&version_compile);

    std::cout << "Linked version: " << version_linked << std::endl;
    std::cout << "Compile version: " << version_compile << std::endl;

    int result = std::cout.fail();
    return result ? EXIT_FAILURE : EXIT_SUCCESS;
}