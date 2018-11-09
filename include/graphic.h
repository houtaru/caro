#ifndef GRAPHIC_H_DEFINED
#define GRAPHIC_H_DEFINED

#include <ncurses.h>
#include <vector>
#include <string>
#include <utility>

const int DEFAULT_HEIGHT = 25;

const int DEFAULT_WIDTH = 46;

struct rectangle {
    int x, y, w, h;
    void set(int _x, int _y, int _h, int _w) {
        x = _x; y = _y; h = _h; w = _w;
    }
    int top() { return x; }
    int bottom() { return x + h - 1; }
    int left() { return y; }
    int right() { return y + w - 1; }

    int width() { return w; }
    int height() { return h; }

    void drawEdges() {
        attron(A_BOLD);
        for (int i = top(); i <= bottom(); ++i) {
            int x = i - top();
            mvaddch(i, left(), ACS_VLINE);
            mvaddch(i, right(), ACS_VLINE);
        }
        for (int i = left(); i <= right(); ++i) {
            int x = i - left();
            mvaddch(top(), i, ACS_HLINE);
            mvaddch(bottom(), i, ACS_HLINE);
        }
        mvaddch(top(), left(), ACS_DIAMOND);
        mvaddch(top(), right(), ACS_DIAMOND);
        mvaddch(bottom(), left(), ACS_DIAMOND);
        mvaddch(bottom(), right(), ACS_DIAMOND);
    }
    void drawTable() {
        attron(A_BOLD);
        for (int x = top(); x <= bottom(); ++x)
            for (int y = left(); y <= right(); ++y) {
                int h = x - top(), w = y - left();
                if (h % 2 && w % 2) continue;
                if (h % 2) {
                    mvaddch(x, y, ACS_VLINE);
                } else {
                    mvaddch(x, y, (w % 2) ? ACS_HLINE : ACS_DIAMOND);
                }
            }
        attroff(A_BOLD);
    }
    bool inside (std::pair <int, int> p) {
        return top() <= p.first && p.first <= bottom() && left() <= p.second && p.second <= right();
    }
};

namespace GameState {
    struct PlayerState {
        int wins, loses, draws, color;
        bool isCurrentPlayer;
        rectangle screen;
        char name[22];
        char chess;

        PlayerState();

        void init(int x, int y, int h, int w, int _color, char _chess, char _name[22]);
        
        void setColor(int x);
        
        void setName(char _name[22]);
        
        void setChess(char c);
        
        void doMove(int x, int y);
        
        void isWinner();
        
        void isLoser();
        
        void isDraw();
        
        void print();
    };

    PlayerState player[2];

    void setup(int _m = DEFAULT_HEIGHT, int _n = DEFAULT_WIDTH);
    
    void print();
    
    bool haveWinner();
    
    bool canMove();
    
    void updateData();
};

namespace ObjectFall {
    struct Object {
        std::pair <int, int> pos;
        char icon;
        
        Object(int, int, char);
        
        void update();

        std::pair <int, int> getPos() const;
        char getIcon() const;
    };

    //rectangle field;
    //std::vector <Object> object_set;

    void update();

    void erase(int x);

    std::vector <Object> getData();

    void setBound(rectangle);
}

namespace Graphic {
    namespace Screens {
        //rectangle demoStoryScreen;
        //rectangle mainScreen, headerWindow, menuWindow;
        //rectangle statisticWindow, PvCStatisticWindow;
        //rectangle optionMenuWindow;

        //int currentScreen;
        //int currentMainPtr;
        
        void init();
        void load();

        void Clear(int x, int y, int h, int w);
        void sketchScreen();
        
        void sketchDemoStory();

        void sketchMainScreen();

        void sketchGameScreen();
        
        void sketchStatisticScreen();
        
        void sketchOptionScreen();

        void updateMainPtr(int x);
    };
    namespace Color {
        // 0/1 : Background color
        // 2 : player edges color
        // 3 : player one chess color
        // 4 : player two chess color

        void init();
        int getCurrentBackgoundColor();
        void colorOn(int x, int y);
        void colorOff(int x, int y);
    }


    void init();

    void load();

    void update();
}

#endif