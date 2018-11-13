#include <ui.h>
#include <data.h>
#include <machine.h>
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

extern int Ui::Controler::soundState;

void Ui::Controler::process() {
    if (Ui::Input::isArrowKey()) arrowKeyProcess();
    
    if (Ui::Input::isEnterKey()) enterKeyProcess();
    
    if (Ui::Input::is_Z_Key()) backSpaceKeyProcess();

    if ((Graphic::Screens::getCurrentScreen() == STATISTIC_SCREEN || Graphic::Screens::getCurrentScreen() == OPTION_SCREEN) && Input::is_B_Key()) {
        Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
    }
    if (Ui::Input::is_U_Key()) undoProcess();
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
    
    if (Graphic::Screens::getCurrentScreen() == OPTION_SCREEN) {
        Graphic::Screens::updatePtr(OPTION_PTR, -1);
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
        Graphic::Screens::updatePtr(STATIS_PTR, 1);
    }
    if (Graphic::Screens::getCurrentScreen() == OPTION_SCREEN) {
        Graphic::Screens::updatePtr(OPTION_PTR, 1);
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
    } else 
    if (Graphic::Screens::getCurrentScreen() == PVP_SCREEN) {
        if (GameState::canMove() == false) {
            GameState::updateData();
            
            while (true) {
                Input::read();
                if (Input::is_R_Key()) {
                    GameState::reset();

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
                    GameState::reset();
                    GameState::print();
                    break;
                }
                if (Input::is_Z_Key()) {
                    Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
                    break;
                }
            }
        } else {
            if (GameState::doMove() == false) return;
            
            if (GameState::haveWinner()) {
                Data::Statis::saveGame(STATISTIC_PVC);
                GameState::updateData();
                while (true) {
                    Input::read();
                    if (Input::is_R_Key()) {
                        GameState::reset();
                        GameState::print();
                        return;
                    }
                    if (Input::is_Z_Key()) {
                        GameState::reset(true);
                        Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
                        return;
                    }
                }
            }
            refresh();
            
            Machine::doMove();
            refresh();

            if (GameState::haveWinner()) {
                Data::Statis::saveGame(STATISTIC_PVC);
                GameState::updateData();
                while (true) {
                    Input::read();
                    if (Input::is_R_Key()) {
                        GameState::reset();
                        GameState::print();
                        return;
                    }
                    if (Input::is_Z_Key()) {
                        GameState::reset(true);
                        Graphic::Screens::updateCurrentScreen(MAIN_SCREEN);
                        return;
                    }
                }
            }
            refresh();
        }
    } else 
    if (Graphic::Screens::getCurrentScreen() == STATISTIC_SCREEN) {
        if (Graphic::Screens::getPtr(STATIS_PTR) + 3 == STATISTIC_PVP) {
            Graphic::Screens::sketchStatisPVPScreen();
            Ui::Controler::PvPStatisControl();
        } else {
            Graphic::Screens::sketchStatisPVCScreen();
            Ui::Controler::PvCStatisControl();
        }
    } else
    if (Graphic::Screens::getCurrentScreen() == OPTION_SCREEN) {
        int x = Graphic::Screens::getPtr(OPTION_PTR);
        if (x == 0) {
            Graphic::Screens::sketchSoundScreen();
            soundControl();
        }
        if (x == 1) {
            Graphic::Screens::sketchSizeScreen();
            sizeControl();
        }
        if (x == 2) {
            Graphic::Screens::sketchIconScreen();
            iconControl();
        }
    }
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

void Ui::Controler::PvPStatisControl() {
    int ptr = 0, sz = Data::Statis::getStatisSize();
    while (true) {
        Input::read();
        if (Input::is_B_Key()) {
            Graphic::Screens::sketchMainWindow();
            Graphic::Screens::sketchStatisticScreen();
            break;
        }
        if (Input::isEnterKey()) {
            Data::Statis::getState(STATISTIC_PVP, ptr);
            ptr = 0;
        }
        if (Input::isKeyUp()) {
            mvaddstr(Graphic::Screens::subscreens[STATISTIC_PVP].top() + 2 + ptr, Graphic::Screens::subscreens[STATISTIC_PVP].left() + 1, Data::Statis::getStatisName(ptr).c_str());
            ptr = (ptr - 1 + sz) % sz;
            Graphic::Color::reverseOn(); attron(A_BOLD);
            mvaddstr(Graphic::Screens::subscreens[STATISTIC_PVP].top() + 2 + ptr, Graphic::Screens::subscreens[STATISTIC_PVP].left() + 1, Data::Statis::getStatisName(ptr).c_str());
            Graphic::Color::reverseOff(); attroff(A_BOLD);
        }
        if (Input::isKeyDown()) {
            mvaddstr(Graphic::Screens::subscreens[STATISTIC_PVP].top() + 2 + ptr, Graphic::Screens::subscreens[STATISTIC_PVP].left() + 1, Data::Statis::getStatisName(ptr).c_str());
            ptr = (ptr + 1) % sz;
            Graphic::Color::reverseOn(); attron(A_BOLD);
            mvaddstr(Graphic::Screens::subscreens[STATISTIC_PVP].top() + 2 + ptr, Graphic::Screens::subscreens[STATISTIC_PVP].left() + 1, Data::Statis::getStatisName(ptr).c_str());
            Graphic::Color::reverseOff(); attroff(A_BOLD);
        }
        refresh();
    }
}

void Ui::Controler::PvCStatisControl() {
    int ptr = 0, sz = Data::Statis::getStatisSize();
    while (true) {
        Input::read();
        if (Input::is_B_Key()) {
            Graphic::Screens::sketchMainWindow();
            Graphic::Screens::sketchStatisticScreen();
            break;
        }
        if (Input::isEnterKey()) {
            Data::Statis::getState(STATISTIC_PVC, ptr);
            ptr = 0;
        }
        if (Input::isKeyUp()) {
            mvaddstr(Graphic::Screens::subscreens[STATISTIC_PVC].top() + 2 + ptr, Graphic::Screens::subscreens[STATISTIC_PVC].left() + 1, Data::Statis::getStatisName(ptr).c_str());
            ptr = (ptr - 1 + sz) % sz;
            Graphic::Color::reverseOn(); attron(A_BOLD);
            mvaddstr(Graphic::Screens::subscreens[STATISTIC_PVC].top() + 2 + ptr, Graphic::Screens::subscreens[STATISTIC_PVC].left() + 1, Data::Statis::getStatisName(ptr).c_str());
            Graphic::Color::reverseOff(); attroff(A_BOLD);
        }
        if (Input::isKeyDown()) {
            mvaddstr(Graphic::Screens::subscreens[STATISTIC_PVC].top() + 2 + ptr, Graphic::Screens::subscreens[STATISTIC_PVC].left() + 1, Data::Statis::getStatisName(ptr).c_str());
            ptr = (ptr + 1) % sz;
            Graphic::Color::reverseOn(); attron(A_BOLD);
            mvaddstr(Graphic::Screens::subscreens[STATISTIC_PVC].top() + 2 + ptr, Graphic::Screens::subscreens[STATISTIC_PVC].left() + 1, Data::Statis::getStatisName(ptr).c_str());
            Graphic::Color::reverseOff(); attroff(A_BOLD);
        }
        refresh();
    }
}

void Ui::Controler::undoProcess() {
    if (Graphic::Screens::getCurrentScreen() == PVC_SCREEN || Graphic::Screens::getCurrentScreen() == PVP_SCREEN) {
        GameState::undoProcess();
    }
}

void Ui::Controler::soundControl() {

}