#include <ui.h>
#include <data.h>
#include <utils.h>
#include <graphic.h>
#include <GameState.h>

#include <tuple>
#include <fstream>
#include <algorithm>

using namespace std;

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
    "", "", "", "", "", "", "", "", "",
    "   GOMOKU v1.0.0   ",
    " Author: The San Cao",
    " Language using: C++"
};

const std::vector <std::string> statisMenu {
    "  Play with Friend  ",
    " Play with Computer "
};

const std::vector < std::vector < std::string > > optionMenu {
    {"       Sound        ",
     "         On         ", 
     "        Off         ",},
    {"       Size         "},
    {"       Icon         "}
};

//ObjectFall Definition
rectangle ObjectFall::field;
vector <ObjectFall::Object> ObjectFall::object_set;

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


//Graphic Definition

//Graphic Variables
rectangle Graphic::Screens::screens[6];
rectangle Graphic::Screens::subscreens[5];

int Graphic::Screens::currentPtr[3];
int Graphic::Screens::currentScreen;

int Graphic::Color::colors[2][5];
int Graphic::Color::currentBackgroundColor;

//Graphic Function Definition
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
    for (int i = 0; i < 2; ++i) for (int j = 0; j < 5; ++j) if (i != j)
        Color::colors[i][j] = ++cnt;
    
    init_pair(Color::colors[0][1], COLOR_WHITE, COLOR_BLACK);
    init_pair(Color::colors[0][2], COLOR_YELLOW, COLOR_BLACK);
    init_pair(Color::colors[0][3], COLOR_RED, COLOR_BLACK);
    init_pair(Color::colors[0][4], COLOR_BLUE, COLOR_BLACK);
    init_pair(Color::colors[1][0], COLOR_BLACK, COLOR_WHITE);
    init_pair(Color::colors[1][2], COLOR_YELLOW, COLOR_WHITE);
    init_pair(Color::colors[1][3], COLOR_RED, COLOR_WHITE);
    init_pair(Color::colors[1][4], COLOR_BLUE, COLOR_WHITE);
}

void Graphic::load() {
    init();
    Screens::init();
    Color::init();
    Screens::sketchScreen();
}

void Graphic::Screens::init() {
    currentScreen = DEMO_STORY_SCREEN;
    currentPtr[MAIN_PTR] = currentPtr[STATIS_PTR] = 0;

    screens[DEMO_STORY_SCREEN].set(LINES / 2 - 9, COLS / 2 - 40, 7, 70);

    screens[MAIN_SCREEN].set(0, 0, LINES, COLS);
    subscreens[MAIN_HEADER].set(max(4, screens[MAIN_SCREEN].height() / 2 - 14), max(5, screens[MAIN_SCREEN].width() / 2 - 34), 7, 69);
    subscreens[MAIN_MENU].set(subscreens[MAIN_HEADER].bottom() + 5, subscreens[MAIN_HEADER].left() + 22, 15, 20);

    screens[STATISTIC_SCREEN].set(subscreens[MAIN_HEADER].top() + 9, subscreens[MAIN_HEADER].left() + 22, 9, 22);
    subscreens[STATISTIC_PVP].set(subscreens[MAIN_HEADER].top() + 9, subscreens[MAIN_HEADER].left() + 22, 9, 22);
    subscreens[STATISTIC_PVC].set(subscreens[MAIN_HEADER].top() + 9, subscreens[MAIN_HEADER].left() + 22, 9, 22);

    screens[OPTION_SCREEN].set(subscreens[MAIN_HEADER].top() + 9, subscreens[MAIN_HEADER].left() + 22, 9, 22);
}

void Graphic::Screens::Clear(int x, int y, int h, int w) {
    for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j)
        mvaddch(x + i, y + j, ' ');
    refresh();
}

void Graphic::Screens::sketchScreen() {
    if (currentScreen != OPTION_SCREEN && currentScreen != STATISTIC_SCREEN) Clear(0, 0, LINES, COLS);
    if (currentScreen == DEMO_STORY_SCREEN) {
        sketchDemoStory();
        sketchScreen();
    }
    if (currentScreen == PVP_SCREEN || currentScreen == PVC_SCREEN) {
        GameState::setTypeGame(currentScreen);
        sketchGameScreen();
    }
    if (currentScreen == STATISTIC_SCREEN) sketchStatisticScreen();
    if (currentScreen == OPTION_SCREEN) sketchOptionScreen();
    if (currentScreen == MAIN_SCREEN) sketchMainWindow();
    refresh();
}

void Graphic::Screens::sketchDemoStory() {
    screens[DEMO_STORY_SCREEN].drawEdges();
    ObjectFall::setBound(screens[MAIN_SCREEN]);
    int i = 0, j = 0, x = screens[DEMO_STORY_SCREEN].top() + 2, y = screens[DEMO_STORY_SCREEN].left() + 2, timer = 0;
    while (true) {
        Ui::Input::read();
        if (i >= demoStory.size()) break;
        
        for (auto it : ObjectFall::getData()) if (!screens[DEMO_STORY_SCREEN].inside(it.getPos())) {
            mvaddch(it.getPos().first, it.getPos().second, ' ');
        }
        if (timer % 3 == 0) ObjectFall::update();
        for (auto it : ObjectFall::getData()) if (!screens[DEMO_STORY_SCREEN].inside(it.getPos())) {
            mvaddch(it.getPos().first, it.getPos().second, it.getIcon());
        }

        if (j < demoStory[i].size()) {
            attron(A_BOLD);
            mvaddch(x, y++, demoStory[i][j++]);
            attroff(A_BOLD);
        }
        if ((i == 1 && j == 23) || (i == 4 && j == 26)) {
            x += 2; y = screens[DEMO_STORY_SCREEN].left() + 2 + 5;
        }

        if (Ui::Input::isEnterKey()) {
            Clear(screens[DEMO_STORY_SCREEN].top() + 1, screens[DEMO_STORY_SCREEN].left() + 1, screens[DEMO_STORY_SCREEN].height() - 2, screens[DEMO_STORY_SCREEN].width() - 2);
            i++; j = 0;
            x = screens[DEMO_STORY_SCREEN].top() + 2, y = screens[DEMO_STORY_SCREEN].left() + 2;
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

void Graphic::Screens::sketchMainWindow() {
    for (int i = 0; i < (int) header.size(); ++i) 
        for (int j = 0; j < (int) header[i].size(); ++j) 
            mvaddch(subscreens[MAIN_HEADER].top() + 1 + i, subscreens[MAIN_HEADER].left() + 1 + j, header[i][j]);
    
    for (int i = 0; i < (int) mainMenu.size(); ++i) {
        if (i == currentPtr[MAIN_PTR]) {
            Color::reverseOn();
            attron(A_BOLD);
        }
        mvaddstr(subscreens[MAIN_MENU].top() + i, subscreens[MAIN_MENU].left(), mainMenu[i].c_str());
        if (i == currentPtr[MAIN_PTR]) {
            attroff(A_BOLD);
            Color::reverseOff();
        }
    }
}

void Graphic::Screens::sketchGameScreen() {
    attron(A_BOLD);
    mvaddstr(22, 102, "Arrow key : move cursor");
    mvaddstr(24, 104, "Z : Return to main");
    mvaddstr(26, 109, "U: Undo");
    mvaddstr(28, 102, "Space/Enter: take turn");
    attroff(A_BOLD);

    GameState::print();
    if (Data::Save::canLoadGame(GameState::getTypeGame())) {
        Clear(GameState::getBoardHeight() / 2 - 2, GameState::getBoardWidth() / 2 - 30, 3, 60);
        
        attron(A_BOLD);
        
        mvprintw(GameState::getBoardHeight() / 2 - 1, GameState::getBoardWidth() / 2 - 10, "Continue         New Game");
        
            //Graphic::Screens::sketchStatisPVPScreen();
        Color::reverseOn();
        mvaddch(GameState::getBoardHeight() / 2 - 1, GameState::getBoardWidth() / 2 - 10, 'C');
        mvaddch(GameState::getBoardHeight() / 2 - 1, GameState::getBoardWidth() / 2 - 10 + 17, 'N');
        Color::reverseOff();

        attroff(A_BOLD);

        while (true) {
            Ui::Input::read();
            if (Ui::Input::is_C_Key()) {
                Data::Save::loadGame(GameState::getTypeGame());
                break;
            }
            if (Ui::Input::is_N_Key()) {
                break;
            }
        }
    }
    GameState::print();
}

void Graphic::Screens::sketchStatisticScreen() {
    Clear(screens[STATISTIC_SCREEN].top(), screens[STATISTIC_SCREEN].left(), screens[STATISTIC_SCREEN].height(), screens[STATISTIC_SCREEN].width());
    screens[STATISTIC_SCREEN].drawEdges();
    for (int i = 0; i < (int) statisMenu.size(); ++i) {
        if (i == currentPtr[STATIS_PTR]) {
            attron(A_BOLD);
            Color::reverseOn();
        }
        mvaddstr(screens[STATISTIC_SCREEN].top() + 3 + i, screens[STATISTIC_SCREEN].left() + 1, statisMenu[i].c_str());
        if (i == currentPtr[STATIS_PTR]) {
            attroff(A_BOLD);
            Color::reverseOff();
        }
    }
    attron(A_BOLD | A_BLINK);
    mvaddstr(screens[STATISTIC_SCREEN].bottom() + 2, screens[STATISTIC_SCREEN].left() + 2, "Press B to return");
    attroff(A_BOLD | A_BLINK);
}

void Graphic::Screens::sketchStatisPVPScreen(bool flag) {
    Clear(subscreens[STATISTIC_PVP].top(), subscreens[STATISTIC_PVP].left(), subscreens[STATISTIC_PVP].height(), subscreens[STATISTIC_PVP].width());
    subscreens[STATISTIC_PVP].drawEdges();
    
    Data::Statis::loadStatis(STATISTIC_PVP);
    int sz = Data::Statis::getStatisSize();

    for (int i = 0; i < sz; ++i) {
        if (i == 0) {
            attron(A_BOLD);
            Color::reverseOn();
        }
        mvaddstr(subscreens[STATISTIC_PVP].top() + 2 + i, subscreens[STATISTIC_PVP].left() + 1, Data::Statis::getStatisName(i).c_str());
        if (i == 0) {
            attroff(A_BOLD);
            Color::reverseOff();
        }
    }
    refresh();
}

void Graphic::Screens::sketchStatisPVCScreen(bool flag) {
    Clear(subscreens[STATISTIC_PVC].top(), subscreens[STATISTIC_PVC].left(), subscreens[STATISTIC_PVC].height(), subscreens[STATISTIC_PVC].width());
    subscreens[STATISTIC_PVC].drawEdges();
    
    Data::Statis::loadStatis(STATISTIC_PVC);
    int sz = Data::Statis::getStatisSize();

    for (int i = 0; i < sz; ++i) {
        if (i == 0) {
            attron(A_BOLD);
            Color::reverseOn();
        }
        mvaddstr(subscreens[STATISTIC_PVP].top() + 2 + i, subscreens[STATISTIC_PVP].left() + 1, Data::Statis::getStatisName(i).c_str());
        if (i == 0) {
            attroff(A_BOLD);
            Color::reverseOff();
        }
    }
    refresh();
}

void Graphic::Screens::sketchOptionScreen() {
    Clear(screens[OPTION_SCREEN].top(), screens[OPTION_SCREEN].left(), screens[OPTION_SCREEN].height(), screens[OPTION_SCREEN].width());
    screens[OPTION_SCREEN].drawEdges();
    int ptr = 0;
    for (int i = 0; i < optionMenu.size(); ++i) {
        if (i == 0) {
            Color::reverseOn(); attron(A_BOLD);
        }
        mvaddstr(screens[OPTION_SCREEN].top() + 3 + i, screens[OPTION_SCREEN].left() + 1, optionMenu[i][0].c_str());
        if (i == 0) {
            Color::reverseOff(); attroff(A_BOLD);
        }
    }
    refresh();
}

void Graphic::Screens::sketchScreen() {
    Clear(screens[OPTION_SCREEN].top(), screens[OPTION_SCREEN].left(), screens[OPTION_SCREEN].height(), screens[OPTION_SCREEN].width());
    screens[OPTION_SCREEN].drawEdges();

    if (Ui::Controler::soundState == 0)
}

void Graphic::Screens::updateCurrentScreen(int x) { 
    currentScreen = x;
    sketchScreen();
}

void Graphic::Screens::updatePtr(int id, int x) {
    if (id == MAIN_PTR) {
        mvaddstr(subscreens[MAIN_MENU].top() + currentPtr[MAIN_PTR], subscreens[MAIN_MENU].left(), mainMenu[currentPtr[MAIN_PTR]].c_str());
        currentPtr[MAIN_PTR] = (currentPtr[MAIN_PTR] + x + 4) % 4;
        
        attron(A_BOLD); Color::reverseOn();
        
        mvaddstr(subscreens[MAIN_MENU].top() + currentPtr[MAIN_PTR], subscreens[MAIN_MENU].left(), mainMenu[currentPtr[MAIN_PTR]].c_str());
        
        attroff(A_BOLD); Color::reverseOff();
    }
    if (id == STATIS_PTR) {
        mvaddstr(screens[STATISTIC_SCREEN].top() + 3 + currentPtr[STATIS_PTR], screens[STATISTIC_SCREEN].left() + 1, statisMenu[currentPtr[STATIS_PTR]].c_str());
        currentPtr[STATIS_PTR] = (currentPtr[STATIS_PTR] + x + 2) % 2;
        
        attron(A_BOLD); Color::reverseOn();
        
        mvaddstr(screens[STATISTIC_SCREEN].top() + 3 + currentPtr[STATIS_PTR], screens[STATISTIC_SCREEN].left() + 1, statisMenu[currentPtr[STATIS_PTR]].c_str());
        
        attroff(A_BOLD); Color::reverseOff();
    }
    if (id == OPTION_PTR) {
        mvaddstr(screens[OPTION_SCREEN].top() + 3 + currentPtr[STATIS_PTR], screens[OPTION_SCREEN].left() + 1, statisMenu[currentPtr[OPTION_PTR]].c_str());
        currentPtr[OPTION_PTR] = (currentPtr[OPTION_PTR] + x + 3) % 3;
        
        attron(A_BOLD); Color::reverseOn();
        
        mvaddstr(screens[OPTION_SCREEN].top() + 3 + currentPtr[STATIS_PTR], screens[OPTION_SCREEN].left() + 1, statisMenu[currentPtr[OPTION_PTR]].c_str());
        
        attroff(A_BOLD); Color::reverseOff();
    }
}

int Graphic::Screens::getPtr(int id) { return currentPtr[id];}

int Graphic::Screens::getCurrentScreen() { return currentScreen; }

void Graphic::Color::init() {
    currentBackgroundColor = 0;
}

void Graphic::Color::reverseColorOn(int x) {
    attron(COLOR_PAIR(colors[1 - currentBackgroundColor][x]));
}

void Graphic::Color::reverseColorOff(int x) {
    attroff(COLOR_PAIR(colors[1 - currentBackgroundColor][x]));
}

void Graphic::Color::colorOn(int x) {
    attron(COLOR_PAIR(colors[currentBackgroundColor][x]));
}

void Graphic::Color::colorOff(int x) {
    attroff(COLOR_PAIR(colors[currentBackgroundColor][x]));
}

void Graphic::Color::reverseOn() {
    attron(COLOR_PAIR(colors[1 - currentBackgroundColor][currentBackgroundColor]));
}

void Graphic::Color::reverseOff() {
    attroff(COLOR_PAIR(colors[1 - currentBackgroundColor][currentBackgroundColor]));
}

void Graphic::Color::setCurrentBackgroundColor(int x) { currentBackgroundColor = x; }
