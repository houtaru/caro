#ifndef UI_H_DEFINED
#define UI_H_DEFINED

#include <ncurses.h>

namespace Ui {
    namespace Input {
        extern int input;
        void read();
        
        bool isArrowKey();
        
        bool isKeyLeft();
        
        bool isKeyRight();
        
        bool isKeyUp();
        
        bool isKeyDown();

        bool isEnterKey();

        bool isEscKey();        

        bool is_Q_Key();
        
        bool is_R_Key();

        bool is_S_Key();

        bool is_Z_Key();

        bool is_U_Key();

        bool is_G_Key();

        bool is_C_Key();

        bool is_N_Key();

        bool is_B_Key();

        bool isDigit();

        bool isAlpha();

        int getInput();
    };
    namespace Controler {
        extern int soundState;

        void process();
        
        void arrowKeyProcess();

        void keyUpProcess();

        void keyDownProcess();

        void keyLeftProcess();

        void keyRightProcess();
        
        void enterKeyProcess();

        void backSpaceKeyProcess();

        void PvPStatisControl();

        void PvCStatisControl();

        void undoProcess();

        void soundControl();

        void sizeControl();
        
        void iconControl();

        int makeSound();

        void setSound(int x);
    };
};

#endif