#ifndef MACHINE_H_DEFINED
#define MACHINE_H_DEFINED

#include <vector>

namespace Machine {
    std::vector <int> check(int x, int y, int icon_);

    std::pair <int, int> getMove();

    void doMove();
}

#endif