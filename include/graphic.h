#ifndef GRAPHIC_H_DEFINED
#define GRAPHIC_H_DEFINED

#include <ncurses.h>
#include <vector>
#include <string>
#include <utility>

enum SCREENS {
    PVP_SCREEN,
    PVC_SCREEN,
    STATISTIC_SCREEN,
    OPTION_SCREEN,
    MAIN_SCREEN,
    DEMO_STORY_SCREEN
};

enum SUBSCREENS {
    MAIN_HEADER,
    MAIN_MENU,
    STATISTIC_MENU,
    STATISTIC_PVP,
    STATISTIC_PVC
};

enum POINTERS {
    MAIN_PTR,
    STATIS_PTR
};

#define EDGES_COLOR 2
#define PLAYER_ONE_COLOR 3
#define PLAYER_TWO_COLOR 4

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
        //attron(A_BOLD);
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
        //attroff(A_BOLD);
    }
    bool inside (std::pair <int, int> p) {
        return top() <= p.first && p.first <= bottom() && left() <= p.second && p.second <= right();
    }
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

    extern rectangle field;
    extern std::vector <Object> object_set;

    void update();

    void erase(int x);

    std::vector <Object> getData();

    void setBound(rectangle);
}

namespace Graphic {
    namespace Screens {
        extern rectangle screens[6];
        extern rectangle subscreens[5];
        extern int currentScreen;
        extern int currentPtr[2];
        
        void init();
        void load();

        void Clear(int x, int y, int h, int w);

        void sketchScreen();
        
        void sketchDemoStory();

        void sketchMainWindow();

        void sketchGameScreen();
        
        void sketchStatisticScreen();

        void sketchStatisPVPScreen(bool flag = true);

        void sketchStatisPVCScreen();
        
        void sketchOptionScreen();

        void updatePtr(int id, int x);

        int getPtr(int id);

        void updateCurrentScreen(int x);

        int getCurrentScreen();
    };
    namespace Color {
        extern int currentBackgroundColor;
        extern int colors[2][5];

        void init();
        
        void setCurrentBackgroundColor(int x);
        
        void colorOn(int x);
        
        void colorOff(int x);
        
        void reverseColorOn(int x);
        
        void reverseColorOff(int x);
        
        void reverseOn();
        
        void reverseOff();
    }


    void init();

    void load();

    void update();
}

#endif