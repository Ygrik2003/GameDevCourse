#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

class printer
{
public:
    printer(boost::asio::io_context& io);

    ~printer();

    void print1();
    void print2();

private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer                                   timer1_;
    boost::asio::steady_timer                                   timer2_;
    int                                                         count_;
};

int greeteng();