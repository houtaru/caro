#include <utils.h>
#include <ctime>       // time()
#include <unistd.h>   //usleep()
#include <algorithm>

void Utils::Random::seed() {
    srand(time(NULL));
}

int Utils::Random::get(int l, int r) { 
    return l + rand() % (r - l + 1); 
}

void Utils::Times::delay(int ms) {
    usleep(ms);
}