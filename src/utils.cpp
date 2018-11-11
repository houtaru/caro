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

std::string Utils::Times::getDateTime() {
    std::time_t rawTime;
    std::tm* timeInfo;
    char buffer[80];
    
    std::time(&rawTime);
    timeInfo = std::localtime(&rawTime);

    std::strftime(buffer, 80, "%Y.%m.%d %H:%M:%S", timeInfo);
    return std::string(buffer);
}