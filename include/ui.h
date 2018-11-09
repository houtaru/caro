#ifndef UI_H_DEFINED
#define UI_H_DEFINED

#include <ncurses.h>

namespace Ui {
    namespace Input {
        int input;
        void read();
        
        bool isArrowKey();
        
        bool isKeyLeft();
        
        bool isKeyRight();
        
        bool isKeyUp();
        
        bool isKeyDown();

        bool isEnterKey();

        bool is_Q_Key();
        
        bool is_R_Key();

        bool is_S_Key();

        bool is_Z_Key();

        bool is_U_Key();

        bool is_G_Key();

        bool is_C_Key();

        bool is_N_Key();

        bool is_B_Key();
    };
    namespace Controler {
        void process();
    };
};

#endif