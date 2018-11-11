#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <string>

namespace Utils {
    namespace Random {
        void seed();
        
        int get(int l, int r);
    };

    namespace Times {
        void delay(int ms);
        std::string getDateTime();
    };
};

#endif