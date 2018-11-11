#include <ui.h>
#include <data.h>
#include <graphic.h>
#include <GameState.h>

int Ui::Input::input;

void Ui::Input::read() { input = getch(); }

bool Ui::Input::isArrowKey() { return input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT; }

bool Ui::Input::isKeyUp() { return input == KEY_UP; }

bool Ui::Input::isKeyDown() { return input == KEY_DOWN; }

bool Ui::Input::isKeyLeft() { return input == KEY_LEFT; }

bool Ui::Input::isKeyRight() { return input == KEY_RIGHT; }

bool Ui::Input::isEnterKey() { return input == '\n' || input == ' '; }

bool Ui::Input::isEscKey() { return input == 27; }

bool Ui::Input::is_Q_Key() { return input == 'q' || input == 'Q'; }

bool Ui::Input::is_Z_Key() { return input == 'z' || input == 'Z'; }

bool Ui::Input::is_R_Key() { return input == 'r' || input == 'R'; }

bool Ui::Input::is_S_Key() { return input == 'S' || input == 's'; }

bool Ui::Input::is_C_Key() { return input == 'c' || input == 'C'; }

bool Ui::Input::is_B_Key() { return input == 'B' || input == 'b'; }

bool Ui::Input::is_N_Key() { return input == 'n' || input == 'N'; }

bool Ui::Input::is_U_Key() { return input == 'u' || input == 'U'; }

bool Ui::Input::is_G_Key() { return input == 'G' || input == 'g'; }

void Ui::Controler::process() {
    if (Ui::Input::isArrowKey()) arrowKeyProcess();
    
    if (Ui::Input::isEnterKey()) enterKeyProcess();
    
    if (Ui::Input::is_Z_Key()) backSpaceKeyProcess();
}

void Ui::Controler::arrowKeyProcess() {
    if (Ui::Input::isKeyUp()) keyUpProcess();
    
    if (Ui::Input::isKeyDown()) keyDownProcess();
    
    if (Ui::Input::isKeyLeft()) keyLeftProcess();
    
    if (Ui::Input::isKeyRight()) keyRightProcess();
}

void Ui::Controler::keyUpProcess() {
    if (Graphic::Screens::getCurrentScreen() == MAIN_SCREEN) {
        Graphic::Screens::updatePtr(MAIN_PTR, -1);
    }
    
    if (Graphic::Screens::getCurrentScreen() == PVP_SCREEN || Graphic::Screens::getCurrentScreen() == PVC_SCREEN) {
        GameState::Moving(-1, 0);
    }
    
    if (Graphic::Screens::getCurrentScreen() == STATISTIC_SCREEN) {
        Graphic::Screens::updatePtr(STATIS_PTR, -1);
    }
}

void Ui::Controler::keyDownProcess() {
    if (Graphic::Screens::getCurrentScreen() == MAIN_SCREEN) {
        Graphic::Screens::updatePtr(MAIN_PTR, 1);
    }

    if (Graphic::Screens::getCurrentScreen() == PVP_SCREEN || Graphic::Screens::getCurrentScreen() == PVC_SCREEN) {
        GameState::Moving(1, 0);
    }
    
    if (Graphic::Screens::getCurrentScreen() == STATISTIC_SCREEN) {
        Graphic::Screens::updatePtr(STATIS_PTR, -1);
    }
}

void Ui::Controler::keyLeftProcess() {
    if (Graphic::Screens::getCurrentScreen() == PVP_SCREEN || Graphic::Screens::getCurrentScreen() == PVC_SCREEN) {
        GameState::Moving(0, -1);
    }
}

void Ui::Controler::keyRightProcess() {
    if (Graphic::Screens::getCurrentScreen() == PVP_SCREEN || Graphic::Screens::getCurrentScreen() == PVC_SCREEN) {
        GameState::Moving(0, 1);
    }
}

void Ui::Controler::enterKeyProcess() {
    if (Graphic::Screens::getCurrentScreen() == MAIN_SCREEN) {
        Graphic::Screens::updateCurrentScreen(Graphic::Screens::getPtr(MAIN_PTR));
        //Graphic::Screens::sketchScreen();
    } else 
    if (Graphic::Screens::getCurrentScreen() == PVP_SCREEN) {
        if (GameState::canMove() == false) {
            GameState::updateData();
            
            while (true) {
                Input::read();
                if (Input::is_R_Key()) {
                    GameState::print();
                    break;
                }
                if (Input::is_Z_Key()) {
                    Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
                    break;
                }
            }
        } else {
            GameState::doMove();
            
            if (GameState::haveWinner()) {
                Data::Statis::saveGame(STATISTIC_PVP);
                GameState::updateData();
                while (true) {
                    Input::read();
                    if (Input::is_R_Key()) {
                        GameState::reset();

                        GameState::print();
                        
                        break;
                    }
                    if (Input::is_Z_Key()) {
                        GameState::setup();
                        
                        Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
                        
                        break;
                    }
                }
            }
        }
    } else 
    if (Graphic::Screens::getCurrentScreen() == PVC_SCREEN) {
        if (GameState::canMove() == false) {
            GameState::updateData();
            while (true) {
                Input::read();
                if (Input::is_R_Key()) {
                    GameState::print();
                    break;
                }
                if (Input::is_Z_Key()) {
                    Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
                    break;
                }
            }
        } else {
            GameState::doMove();
        }
        //GameState::Machine::doMove();
    } else 
    if (Graphic::Screens::getCurrentScreen() == STATISTIC_SCREEN) {
        if (Graphic::Screens::getPtr(STATIS_PTR) + 3 == STATISTIC_PVP) {
            Graphic::Screens::sketchStatisPVPScreen();
        } else {GameState::print();
            //Graphic::Screens::sketchStatisPVCScreen();
        }
    }
    //Remember to add user interface to option screen function 
}

void Ui::Controler::backSpaceKeyProcess() {
    if (Graphic::Screens::getCurrentScreen() == PVP_SCREEN || Graphic::Screens::getCurrentScreen() == PVC_SCREEN) {
        GameState::backToMainScreen();
        while (true) {
            Input::read();
            if (Input::is_C_Key()) {
                GameState::print();
                break;
            }
            if (Input::is_S_Key()) {
                Data::Save::savedGame(true);
                GameState::reset(true);
                Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
                break;
            }
            if (Input::is_B_Key()) {
                Data::Save::savedGame(false);
                GameState::reset(true);
                Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
                break;
            }
        }
    }
}