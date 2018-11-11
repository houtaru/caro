#include <ui.h>
#include <caro.h>
#include <utils.h>
#include <graphic.h>
#include <GameState.h>

void Global::load() {
    Utils::Random::seed();
    Graphic::load();
    GameState::setup(DEFAULT_HEIGHT, DEFAULT_WIDTH);
}

void Global::process() {
    Graphic::Screens::sketchScreen();
    while (true) {
        Ui::Input::read();
        Ui::Controler::process();
    }
    Global::close();
}

void Global::close() { endwin(); }