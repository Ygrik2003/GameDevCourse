#include <cstdlib>
#include <hello_world.h>

int main(int argc, char** argv)
{
    int res = greeteng();
    return res == 1 ? EXIT_SUCCESS : EXIT_FAILURE;
}