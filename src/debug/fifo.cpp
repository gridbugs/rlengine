#include "debug/fifo.hpp"

#include <iostream>

std::ofstream fifo::cout;
std::ostream &(&fifo::endl)(std::ostream &os) = std::endl;

void fifo::start() {
    fifo::cout.open("/tmp/debugfifo", std::ios::out | std::ios::app);
    //fifo::cout << "test" << fifo::endl;
}

void fifo::stop() {
    fifo::cout.close();
}
