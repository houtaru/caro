#include "ui.h"
#include <caro.h>
#include "utils.h"
#include "graphic.h"

void Global::load() {
    Utils::Random::seed();
    Graphic::load();
}

void Global::process() {
    Graphic::load();
    while (true) {
      //  Ui::Input::read();
    //    Ui::Controler::process();
    }
    Global::saveGame();
    Global::close();
}

void Global::saveGame() {

}

void Global::close() { endwin(); }