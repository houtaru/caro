#include <ui.h>
#include <utils.h>
#include <graphic.h>
#include <algorithm>
#include <fstream>
#include <tuple>

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
    "", "", "", "", "", "",
    "   GOMOKU v1.0.0   ",
    " Author: The San Cao",
    " Language using: C++"
};

const std::vector <std::string> statisMenu {
    "  Play with Friend  ",
    " Play with Computer "
};

const std::vector < std::vector <std::string> > optionMenu {
    {"Sound:", "On", "Off"},
    {"Player Icon:"},
    {"Background: ", "BLACK", "WHITE"},
    {"Size Board:"}
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

int Graphic::Screens::currentPtr[2];
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
    currentScreen = STATISTIC_MENU;
    currentPtr[MAIN_PTR] = currentPtr[1] = 0;

    screens[DEMO_STORY_SCREEN].set(LINES / 2 - 9, COLS / 2 - 40, 7, 70);

    screens[MAIN_SCREEN].set(0, 0, LINES, COLS);
    subscreens[MAIN_HEADER].set(max(4, screens[MAIN_SCREEN].height() / 2 - 14), max(5, screens[MAIN_SCREEN].width() / 2 - 34), 7, 69);
    subscreens[MAIN_MENU].set(subscreens[MAIN_HEADER].bottom() + 5, subscreens[MAIN_HEADER].left() + 22, 15, 20);

    screens[STATISTIC_SCREEN].set(subscreens[MAIN_HEADER].top() + 5, subscreens[MAIN_HEADER].left() + 22, 9, 22);
    subscreens[STATISTIC_PVP].set(subscreens[MAIN_HEADER].top() + 5, subscreens[MAIN_HEADER].left() + 22, 9, 20);
    subscreens[STATISTIC_PVC].set(subscreens[MAIN_HEADER].top() + 5, subscreens[MAIN_HEADER].left() + 22, 9, 20);

    screens[OPTION_SCREEN].set(subscreens[MAIN_HEADER].bottom() + 4, subscreens[MAIN_HEADER].left() + 24, 8, 20);
}

void Graphic::Screens::Clear(int x, int y, int h, int w) {
    for (int i = 0; i < h; ++i) for (int j = 0; j < w; ++j)
        mvaddch(x + i, y + j, ' ');
    refresh();
}

void Graphic::Screens::sketchScreen() {
    if (currentScreen != OPTION_SCREEN) Clear(0, 0, LINES, COLS);
    if (currentScreen == DEMO_STORY_SCREEN) {
        sketchDemoStory();
        sketchScreen();
    }
    if (currentScreen == PVP_SCREEN || currentScreen == PVC_SCREEN) sketchGameScreen(false);
    if (currentScreen == STATISTIC_SCREEN) sketchStatisticScreen();
    //if (currentScreen == OPTION_SCREEN) sketchOptionScreen();
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
        if (timer % 5 == 0) ObjectFall::update();
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

void Graphic::Screens::sketchGameScreen(bool flag = false) {
    if (!flag) {
        GameState::print();
    } else {

    }
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
}

void Graphic::Screens::sketchStatisPVPScreen() {
    Clear(subscreens[STATISTIC_PVP].top(), subscreens[STATISTIC_PVP].left(), subscreens[STATISTIC_PVP].height(), subscreens[STATISTIC_PVP].width());
    subscreens[STATISTIC_PVP].drawEdges();
    
    ifstream inp("history/OldPvPGame");
    string s, t, c;
    int ptr = 0, mm, nn;
    vector < tuple <string, int, int, string> > list;
    while (inp >> s >> t >> mm >> nn >> c) {
        list.emplace_back(s + " " + t, mm, nn, c);
        int x = subscreens[STATISTIC_PVP].top() + 2 + ptr, y = subscreens[STATISTIC_PVP].left() + 2;
        if (ptr == 0) {
            attron(A_BOLD);
            Color::reverseOn();
        }
        mvaddstr(x, y, s.c_str());
        if (ptr == 0) {
            attroff(A_BOLD);
            Color::reverseOff();
        }
        ptr++;
    }
    while (true) {
        Ui::Input::read();
        if (Ui::Input::is_Q_Key()) break;
        if (Ui::Input::isKeyUp()) {
            int x = subscreens[STATISTIC_PVP].top() + 2 + ptr, y = subscreens[STATISTIC_PVP].left() + 2;
            tie(s, mm, nn, c) = list[ptr];
            mvaddstr(x, y, s.c_str());
            ptr = (ptr - 1 + 5) % 5;
            tie(s, mm, nn, c) = list[ptr];
            attron(A_BOLD);
            Color::reverseOn();
            
            mvaddstr(x, y, s.c_str());
            
            attroff(A_BOLD);
            Color::reverseOff();
        }
        if (Ui::Input::isKeyDown()) {
            int x = subscreens[STATISTIC_PVP].top() + 2 + ptr, y = subscreens[STATISTIC_PVP].left() + 2;
            
            tie(s, mm, nn, c) = list[ptr];
            mvaddstr(x, y, s.c_str());
            
            ptr = (ptr - 1 + 5) % 5;
            
            tie(s, mm, nn, c) = list[ptr];
            
            attron(A_BOLD);
            Color::reverseOn();
            
            mvaddstr(x, y, s.c_str());
            
            attroff(A_BOLD);
            Color::reverseOff();
        }
        if (Ui::Input::isEnterKey()) {
            
        }
    }
}

void Graphic::Screens::updateCurrentScreen(int x) { currentScreen = x; }

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


// Player State Definition
GameState::PlayerState::PlayerState(): wins(0), loses(0), draws(0), isCurrentPlayer(false) {}

void GameState::PlayerState::init(int x, int y, int h, int w, bool cur, int _color, char _chess, string _name) {
    screen.set(x, y, h, w);
    wins = loses = draws = 0;
    isCurrentPlayer = cur;
    color = _color;
    chess = _chess;
    name = _name;
}

void GameState::PlayerState::setColor(int x) { color = x; }

void GameState::PlayerState::setName(string _name) { name = _name; }

void GameState::PlayerState::setChess(char c) { chess = c; }

char GameState::PlayerState::getIcon() const { return chess; }

void GameState::PlayerState::doMove(int x, int y) {
    Graphic::Color::reverseColorOn(color);
    mvaddch(x, y, chess);
    Graphic::Color::reverseColorOn(color);
}

void GameState::PlayerState::isWinner() { wins++; }

void GameState::PlayerState::isLoser() { loses++; }

void GameState::PlayerState::isDraw() { draws++; }

void GameState::PlayerState::print() {
    Graphic::Screens::Clear(screen.top(), screen.left(), screen.height(), screen.width());
    if (isCurrentPlayer) {
        Graphic::Color::colorOn(EDGES_COLOR);
    }
    screen.drawEdges();
    if (isCurrentPlayer) {
        Graphic::Color::colorOff(EDGES_COLOR);
    }

    mvaddstr(screen.top() + 3, screen.left() + 2, name.c_str());
    mvprintw(screen.top() + 5, screen.left() + 2, "Win:         %d", wins);
    mvprintw(screen.top() + 7, screen.left() + 2, "Total:       %d ", wins + loses + draws);
    mvprintw(screen.top() + 9, screen.left() + 2, "Percent win: %.1f", (!wins && !loses) ? 0: wins * 100.0 / (wins + loses));
}

//Game State Variables
int GameState::type; // 0/1 : PVP/PVC game

int GameState::n, GameState::m; //size of play table

int GameState::winner;

pair <int, int> GameState::currentPtrPosition; //Current cursor position

vector < pair <int, int> > GameState::turnsList; //save all moves of players

rectangle GameState::playWindow;

int GameState::state[111][111]; //State of match

int GameState::currentPlayer;

GameState::PlayerState GameState::player[2];

void GameState::setup(int _m = DEFAULT_HEIGHT, int _n = DEFAULT_WIDTH) {
    m = _m; n = _n; winner = -1;
    
    type = Graphic::Screens::currentScreen;

    for (int i = 0; i < 111; ++i) for (int j = 0; j < 111; ++j)
        state[i][j] = 0;
    
    playWindow.set(DEFAULT_HEIGHT - m + 1, DEFAULT_WIDTH - n + 1, (m << 1) + 1, (n << 1) + 1);
    
    currentPtrPosition = {playWindow.top() + 1 + m - m % 2, playWindow.left() + 1 + n - n % 2};

    player[0].init(5, 98, 13, 31, true, PLAYER_ONE_COLOR, 'X', "First Player");
    
    player[1].init(33, 98, 13, 31, false, PLAYER_TWO_COLOR, 'O', "Second Player");
}

void GameState::print() {
    rectangle ss; ss.set(1, 1, (DEFAULT_HEIGHT << 1) + 1, (DEFAULT_WIDTH << 1) + 1);
    ss.drawEdges();
    playWindow.drawTable();
    player[0].print();

    if (type == PVP_SCREEN) {
        player[1].print();
    }

    for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) {
        int x = playWindow.top() + 1 + 2 * (i - 1), y = playWindow.left() + 1 +  2 * (j - 1);
        
        if (currentPtrPosition.first == x && currentPtrPosition.second == y) {
            if (state[i][j]) Graphic::Color::reverseColorOn(state[i][j] + 1);
            else Graphic::Color::reverseOn();
        }
        
        if (state[i][j]) mvaddch(x, y, state[i][j] == 1 ? 'X' : 'O');
        else mvaddch(x, y, ' ');
        
        if (currentPtrPosition.first == x && currentPtrPosition.second == y) {
            if (state[i][j]) Graphic::Color::reverseColorOff(state[i][j] + 1);
            else Graphic::Color::reverseOff();
        }
    }
}

void GameState::Moving(int x, int y) {
    int xx = currentPtrPosition.first / 2, yy = currentPtrPosition.second / 2;
    
    if (state[xx][yy]) {
        Graphic::Color::colorOn(2 + state[xx][yy]);
        
        mvaddch(currentPtrPosition.first, currentPtrPosition.second, player[state[xx][yy] - 1].getIcon());
        
        Graphic::Color::colorOff(2 + state[xx][yy]);
    } else {
        mvaddch(currentPtrPosition.first, currentPtrPosition.second, ' ');
    }
    
    currentPtrPosition.first += x << 2; 
    
    currentPtrPosition.first = max(currentPtrPosition.first, playWindow.top() + 1);
    
    currentPtrPosition.first = min(currentPtrPosition.first, playWindow.bottom() - 1);
    
    currentPtrPosition.second += y << 2;
    
    currentPtrPosition.second = max(currentPtrPosition.second, playWindow.left() + 1);
    
    currentPtrPosition.second = min(currentPtrPosition.second, playWindow.right() - 1);

    if (state[xx][yy]) {
        Graphic::Color::reverseColorOn(2 + state[xx][yy]);
        
        mvaddch(currentPtrPosition.first, currentPtrPosition.second, player[state[xx][yy] - 1].getIcon());
        
        Graphic::Color::reverseColorOff(2 + state[xx][yy]);
    } else {
        Graphic::Color::reverseOn();
        mvaddch(currentPtrPosition.first, currentPtrPosition.second, ' ');
        Graphic::Color::reverseOff();
    }
}

void GameState::doMove() {
    int x = currentPtrPosition.first / 2, y = currentPtrPosition.second / 2;
    state[x][y] = currentPlayer;
    Graphic::Color::reverseColorOn(2 + state[x][y]);
    
    mvaddch(currentPtrPosition.first, currentPtrPosition.second, player[state[x][y] - 1].getIcon());
    
    Graphic::Color::reverseColorOff(2 + state[x][y]);
}

void GameState::nextTurn() { currentPlayer = 3 - currentPlayer; }

bool GameState::haveWinner() {
    for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) if (state[i][j]) {
        if (i + 4 <= m) {
            int cnt = 1; for (int k = 1; k <= 4; ++k) cnt += state[i][j] == state[i + k][j];
            if (cnt == 5) {
                winner = state[i][j];
                return true;
            }
        }
        if (j + 4 <= n) {
            int cnt = 1; for (int k = 1; k <= 4; ++k) cnt += state[i][j] == state[i][j + k];
            if (cnt == 5) {
                winner = state[i][j];
                return true;
            }
        }
        if (i + 4 <= m && j + 4 <= n) {
            int cnt = 1; for (int k = 1; k <= 4; ++k) cnt += state[i][j] == state[i + k][j + k];
            if (cnt == 5) {
                winner = state[i][j];
                return true;
            }
        }
        if (i + 4 <= m && j - 4 > 0) {
            int cnt = 1; for (int k = 1; k <= 4; ++k) cnt += state[i][j] == state[i + k][j - k];
            if (cnt == 5) {
                winner = state[i][j];
                return true;
            }
        }
    }
    return false;
}

bool GameState::canMove() {
    for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j)
        if (state[i][j] == 0) return true;
    return false;
}

void GameState::updateData() {
    if (winner != -1) {
        player[winner - 1].isWinner();
        if (type == PVP_SCREEN) player[2 - winner].isLoser();
    } else {
        player[0].isDraw();
        player[1].isDraw();
    }
    Graphic::Screens::Clear(playWindow.height() / 2 - 2, playWindow.width() / 2 - 20, 3, 40);
    attron(A_BOLD | A_BLINK);
    if (type == PVP_SCREEN) {
        if (winner == -1)
            mvprintw(playWindow.height() / 2 - 1, playWindow.width() / 2 - 15, "PLAYER %s WIN! PRESS R TO REPLAY!", player[winner - 1].name);
        else
            mvprintw(playWindow.height() / 2 - 1, playWindow.width() / 2 - 12, "DRAW! PRESS R TO REPLAY!");
    } else {
        if (winner == 1)
            mvprintw(playWindow.height() / 2 - 1, playWindow.width() / 2 - 15, "YOU WIN! PRESS R TO REPLAY!");
        else if (winner != -1)
            mvprintw(playWindow.height() / 2 - 1, playWindow.width() / 2 - 15, "YOU LOSE! PRESS R TO REPLAY!");
        else {
            mvprintw(playWindow.height() / 2 - 1, playWindow.width() / 2 - 12, "DRAW! PRESS R TO REPLAY!");
        }
    }
    attroff(A_BOLD | A_BLINK);   
}

void GameState::backToMainScreen() {
    Graphic::Screens::Clear(playWindow.height() / 2 - 2, playWindow.width() / 2 - 30, 3, 60);
    
    attron(A_BOLD);
    
    mvprintw(playWindow.height() / 2 - 1, playWindow.width() / 2 - 15, "Continue         Save         Back");
    
    Graphic::Color::reverseOn();
    
    mvaddch(playWindow.height() / 2 - 1, playWindow.width() / 2 - 15, 'C');
    
    mvaddch(playWindow.height() / 2 - 1, playWindow.width() / 2 - 15 + 17, 'S');
    
    mvaddch(playWindow.height() / 2 - 1, playWindow.width() / 2 - 15 + 30, 'B');
    
    Graphic::Color::reverseOff();
    
    attroff(A_BOLD);
    
    refresh();
}