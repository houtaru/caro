#include <ui.h>
#include <utils.h>
#include <graphic.h>

using namespace std;

#define DEMO_STORY_SCREEN -1
#define PVP_SCREEN 0
#define PVC_SCREEN 1
#define STATISTIC_SCREEN 2
#define OPTION_SCREEN 3
#define MAIN_SCREEN 4

const std::vector <std::string> demoStory {
    "One day, after trying hard all day on working project...",
    "Instead of coming home, Houtaru decide to walk around the city to relax...",
    "While walking, he see a strange thing....",
    "There is a girl playing caro with a cat...",
    "They are both so cute that He approach and play with them without relizing it...",
};
const std::vector <std::string> header {
    " =======    =======    ==      ==    =======    ==   ===   ==    ==",
    "==         ==     ==   ===    ===   ==     ==   ==  ===    ==    ==",
    "==   ===   ==     ==   ====  ====   ==     ==   ======     ==    ==",
    "==    ==   ==     ==   == ==== ==   ==     ==   ==  ===    ==    ==",
    " =======    =======    ==  ==  ==    =======    ==   ===    ====== "
};

const std::vector <std::string> mainMenu {
    "  Play with Friend  ",
    " Play with Computer ",
    "     Statistics     ",
    "       Option       ",
    "", "", "", "", "", "",
    "   GOMOKU v1.0.0   ",
    " Author: The San Cao",
    " Language using: C++"
};

const std::vector < std::vector <std::string> > optionMenu {
    {"Sound:", "On", "Off"},
    {"Player Icon:"},
    {"Background: ", "BLACK", "WHITE"},
    {"Size Board:"}
};

rectangle demoStoryScreen;
rectangle mainScreen, headerWindow, menuWindow;
rectangle statisticWindow, PvCStatisticWindow;
rectangle optionMenuWindow;

rectangle field;
std::vector <ObjectFall::Object> object_set;

int currentScreen;
int currentMainPtr;

int currentBackgroundColor;
int colors[2][5]; 

int type; // 0/1 : PVP/PVC game
int n, m; //size of play table
int winner;
pair <int, int> currentPtrPosition; //Current cursor position
vector < pair <int, int> > turns; //save all moves of players

rectangle playWindow;
int state[111][111]; //State of match

ObjectFall::Object::Object(int _x, int _y, char c) { pos = make_pair(_x, _y); icon = c; }

void ObjectFall::Object::update() { pos.first++; }

std::pair <int, int> ObjectFall::Object::getPos() const { return pos; }

char ObjectFall::Object::getIcon() const { return icon; }

void ObjectFall::update() {
    for (int i = 0; i < object_set.size(); ++i) {
        if (object_set.at(i).getPos().first >= field.bottom())
            object_set.erase(object_set.begin() + i);
        object_set.at(i).update();
    }

    Object s(field.top(), Utils::Random::get(field.left(), field.right()), (Utils::Random::get(0, 1) ? 'O' : 'X'));
    object_set.push_back(s);
}

void ObjectFall::erase(int i) {
    object_set.erase(object_set.begin() + i);
}

std::vector <ObjectFall::Object> ObjectFall::getData() { return object_set; }

void ObjectFall::setBound(rectangle r) { field = r; }

void Graphic::init() {
    initscr();
    cbreak();
    noecho();
    clear();
    refresh();
    
    curs_set(0);
    keypad(stdscr, true);
    nodelay(stdscr, true);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    start_color();
    if (!has_colors()) {
        printf("ERROR: Terminal does not support color!\n");
        exit(0);
    }

    int cnt = 0;
    for (int i = 0; i < 5; ++i) for (int j = 0; j < 5; ++j) if (i != j)
        colors[i][j] = ++cnt;
    
    init_pair(colors[0][1], COLOR_WHITE, COLOR_BLACK);
    init_pair(colors[0][2], COLOR_YELLOW, COLOR_BLACK);
    init_pair(colors[0][3], COLOR_RED, COLOR_BLACK);
    init_pair(colors[0][4], COLOR_BLUE, COLOR_BLACK);
    init_pair(colors[1][0], COLOR_BLACK, COLOR_WHITE);
    init_pair(colors[1][2], COLOR_YELLOW, COLOR_WHITE);
    init_pair(colors[1][3], COLOR_RED, COLOR_WHITE);
    init_pair(colors[1][4], COLOR_BLUE, COLOR_WHITE);
}

void Graphic::load() {
    init();
    Screens::init();
    Color::init();
    Screens::load();
}

void Graphic::update() {

}

void Graphic::Screens::init() {
    currentScreen = MAIN_SCREEN;
    currentMainPtr = 0;

    demoStoryScreen.set(LINES / 2 - 9, COLS / 2 - 40, 7, 70);

    mainScreen.set(0, 0, LINES, COLS);
    headerWindow.set(max(4, mainScreen.height() / 2 - 14), max(5, mainScreen.width() / 2 - 34), 7, 69);
    menuWindow.set(headerWindow.bottom() + 4, headerWindow.left() + 23, 15, 20);

    optionMenuWindow.set(headerWindow.bottom() + 4, headerWindow.left() + 24, 5, 33);
}

void Graphic::Screens::load() {
    sketchScreen();
}

void Graphic::Screens::Clear(int x, int y, int h, int w) {
    for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j)
        mvaddch(x + i, y + j, ' ');
    refresh();
}

void Graphic::Screens::sketchScreen() {
    if (currentScreen != OPTION_SCREEN) Clear(0, 0, LINES, COLS);
    if (currentScreen == DEMO_STORY_SCREEN) sketchDemoStory();
    //if (currentScreen == PVP_SCREEN || currentScreen == PVC_SCREEN) sketchGameScreen();
    //if (currentScreen == STATISTIC_SCREEN) sketchStatisticScreen();
    //if (currentScreen == OPTION_SCREEN) sketchOptionScreen();
    if (currentScreen == MAIN_SCREEN) sketchMainScreen();
    refresh();
}

void Graphic::Screens::sketchDemoStory() {
    demoStoryScreen.drawEdges(); refresh();
    ObjectFall::setBound(mainScreen);
    int i = 0, j = 0, x = demoStoryScreen.top() + 2, y = demoStoryScreen.left() + 2, timer = 0;
    while (true) {
        Ui::Input::read();
        if (i >= demoStory.size()) break;
        
        for (auto it : ObjectFall::getData()) if (!demoStoryScreen.inside(it.getPos())) {
            mvaddch(it.getPos().first, it.getPos().second, ' ');
        }
        if (timer % 5 == 0) ObjectFall::update();
        for (auto it : ObjectFall::getData()) if (!demoStoryScreen.inside(it.getPos())) {
            mvaddch(it.getPos().first, it.getPos().second, it.getIcon());
        }

        if (j < demoStory[i].size()) {
            attron(A_BOLD);
            mvaddch(x, y++, demoStory[i][j++]);
            attroff(A_BOLD);
        }
        if ((i == 1 && j == 23) || (i == 4 && j == 26)) {
            x += 2; y = demoStoryScreen.left() + 2 + 5;
        }

        if (Ui::Input::isEnterKey()) {
            Clear(demoStoryScreen.top() + 1, demoStoryScreen.left() + 1, demoStoryScreen.height() - 2, demoStoryScreen.width() - 2);
            i++; j = 0;
            x = demoStoryScreen.top() + 2, y = demoStoryScreen.left() + 2;
            continue;
        } else if (Ui::Input::is_Q_Key()) {
            break;
        }

        timer++;
        refresh();
        Utils::Times::delay(10000);
    }
    currentScreen = MAIN_SCREEN;
}

void Graphic::Screens::sketchMainScreen() {
    for (int i = 0; i < (int) header.size(); ++i) 
        for (int j = 0; j < (int) header[i].size(); ++j) 
            mvaddch(headerWindow.top() + 1 + i, headerWindow.left() + 1 + j, header[i][j]);
    
    for (int i = 0; i < (int) mainMenu.size(); ++i) {
        if (i == currentMainPtr) {
            Color::colorOn(1 - Color::getCurrentBackgoundColor(), Color::getCurrentBackgoundColor());
            attron(A_BOLD);
        }
        mvaddstr(menuWindow.top() + i, menuWindow.left(), mainMenu[i].c_str());
        if (i == currentMainPtr) {
            attroff(A_BOLD);
            Color::colorOff(1 - Color::getCurrentBackgoundColor(), Color::getCurrentBackgoundColor());
        }
    }
}

void Graphic::Screens::sketchGameScreen() {
    
}

void Graphic::Screens::updateMainPtr(int x) {
    mvaddstr(menuWindow.top() + currentMainPtr, menuWindow.left(), mainMenu[currentMainPtr].c_str());
    currentMainPtr = (currentMainPtr + x + 4) % 4;
    
    attron(A_BOLD); Color::colorOn(1 - Color::getCurrentBackgoundColor(), Color::getCurrentBackgoundColor());
    
    mvaddstr(menuWindow.top() + currentMainPtr, menuWindow.left(), mainMenu[currentMainPtr].c_str());
    
    attroff(A_BOLD); Color::colorOff(1 - Color::getCurrentBackgoundColor(), Color::getCurrentBackgoundColor());
}

void Graphic::Color::init() {
    currentBackgroundColor = 0;
}

int Graphic::Color::getCurrentBackgoundColor() { return currentBackgroundColor; }

void Graphic::Color::colorOn(int x, int y) {
    attron(COLOR_PAIR(colors[x][y]));
}

void Graphic::Color::colorOff(int x, int y) {
    attroff(COLOR_PAIR(colors[x][y]));
}
