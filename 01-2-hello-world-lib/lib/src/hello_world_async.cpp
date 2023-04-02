#include <hello_world_async.h>

printer::printer(boost::asio::io_context& io)
    : strand_(boost::asio::make_strand(io))
    , timer1_(io, boost::asio::chrono::seconds(1))
    , timer2_(io, boost::asio::chrono::seconds(1))
    , count_(0)
{
    timer1_.async_wait(boost::asio::bind_executor(
        strand_, boost::bind(&printer::print1, this)));
    timer2_.async_wait(boost::asio::bind_executor(
        strand_, boost::bind(&printer::print2, this)));
}

printer::~printer()
{
    std::cout << "Printing exit with count = " << count_ << std::endl;
}

void printer::print1()
{
    if (count_ < 5)
    {
        std::cout << "Timer 1: " << count_ << std::endl;
        ++count_;
        timer1_.expires_at(timer1_.expiry() +
                           boost::asio::chrono::seconds(count_));
        timer1_.async_wait(boost::asio::bind_executor(
            strand_, boost::bind(&printer::print1, this)));
    }
}
void printer::print2()
{
    if (count_ < 5)
    {
        std::cout << "Timer 2: " << count_ << std::endl;
        ++count_;
        timer2_.expires_at(timer2_.expiry() +
                           boost::asio::chrono::seconds(count_));
        timer2_.async_wait(boost::asio::bind_executor(
            strand_, boost::bind(&printer::print2, this)));
    }
}

int greeteng()
{

    std::cout << "Hello world" << std::endl;

    return std::cout.good();
}
