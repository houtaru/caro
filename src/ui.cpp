#include <ui.h>
#include <graphic.h>

//int input;

void Ui::Input::read() { input = getch(); }

bool Ui::Input::isArrowKey() { return input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT; }

bool Ui::Input::isKeyUp() { return input == KEY_UP; }

bool Ui::Input::isKeyDown() { return input == KEY_DOWN; }

bool Ui::Input::isKeyLeft() { return input == KEY_LEFT; }

bool Ui::Input::isKeyRight() { return input == KEY_RIGHT; }

bool Ui::Input::isEnterKey() { return input == '\n' || input == ' '; }

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
    //if (Ui::Input::isArrowKey()) arrowKeyProcess();
}