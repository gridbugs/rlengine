#ifndef _FIFO_HPP_
#define _FIFO_HPP_

#include <fstream>

namespace fifo {
    extern std::ofstream cout;
    extern std::ostream &(&endl)(std::ostream &os);

    void start();
    void stop();
};

#endif
