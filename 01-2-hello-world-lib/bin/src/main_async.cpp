#include <cstdlib>
#include <hello_world_async.h>

int main()
{
    boost::asio::io_context io;

    printer       p(io);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));

    io.run();
    t.join();

    return 0;
}