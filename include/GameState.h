#ifndef GAME_STATE_H_DEFINED
#define GAME_STATE_H_DEFINED

#include <vector>
#include <string>
#include <utility>
#include <graphic.h>

namespace GameState {
    struct PlayerState {
        int wins, loses, draws, color;
        bool isCurrentPlayer;
        rectangle screen;
        char chess;
        std::string name;

        PlayerState();

        void init(int x, int y, int h, int w, bool cur, int _color, char _chess, std::string _name);
        
        void setColor(int x);
        
        void setName(std::string _name);
        
        void setChess(char c);

        char getIcon() const;

        std::string getName();

        void doMove(int x, int y);

        void next();
        
        void isWinner();
        
        void isLoser();
        
        void isDraw();
        
        void printProfile();

        void resetProfile();
    };

    extern int type; // 0/1 : PVP/PVC game
    extern int n, m; //size of play table
    extern int winner;
    extern int currentPlayer;
    extern std::pair <int, int> currentPtrPosition; //Current cursor position
    extern std::vector < std::pair <int, int> > turnsList; //save all moves of players
    
    extern rectangle playWindow;
    extern int state[111][111]; //State of match

    extern PlayerState player[2];

    void setup(int _m = DEFAULT_HEIGHT, int _n = DEFAULT_WIDTH);

    void setTypeGame(int x);

    void setStateRow(int x);

    void setStateCol(int x);

    void setStateAt(int x, int y, int v);

    void setBoardSize(int x, int y);

    int getTypeGame();

    int getStateRow();

    int getStateCol();

    int getStateAt(int x, int y);

    int getBoardHeight();

    int getBoardWidth();

    int getPtrOx();

    int getPtrOy();

    int getPosOx();

    int getPosOy();
        
    void print();

    void reset(bool flag = false);

    void Moving(int x, int y);

    bool doMove();

    void machine(std::pair <int, int> );

    void nextTurn();
    
    bool haveWinner();
    
    bool canMove();
    
    void updateData();

    void backToMainScreen();

    void undoProcess();
};

#endif