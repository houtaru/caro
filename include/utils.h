#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

namespace Utils {
    namespace Random {
        void seed();
        
        int get(int l, int r);
    };

    namespace Times {
        void delay(int ms);
    };
};

#endif