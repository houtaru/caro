#include <data.h>
#include <fstream>
#include <graphic.h>

void Data::savedGame(bool flag) {
    int currentScreen = Graphic::Screens::getCurrentScreen();
    std::ofstream out((currentScreen == PVP_SCREEN) ? "savedPVP.game" : "savedPvC.game");
    
}
