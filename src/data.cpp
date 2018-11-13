#include <ui.h>
#include <data.h>
#include <utils.h>
#include <graphic.h>
#include <GameState.h>

#include <fstream>
#include <algorithm>

std::vector < std::string > Data::Statis::list;

void Data::Save::savedGame(bool flag) {
    int currentScreen = Graphic::Screens::getCurrentScreen();
    std::ofstream out((currentScreen == PVP_SCREEN) ? "history/savedPVP.game" : "history/savedPvC.game");

    out << flag << "\n";

    if (!flag) return;
    
    out << currentScreen << "\n";
    
    out << GameState::getStateRow() << " " << GameState::getStateCol() << "\n";

    for (int i = 1; i <= GameState::getStateRow(); ++i)
        for (int j = 1; j <= GameState::getStateCol(); ++j)
            out << GameState::getStateAt(i, j) << (j == GameState::getStateCol() ? "\n" : " ");
    
    out.close();
}

bool Data::Save::canLoadGame(int flag) {
    std::ifstream inp((flag == PVP_SCREEN) ? "history/savedPVP.game" : "history/savedPvC.game");
    if (!inp) return false;
    int saved; inp >> saved;
    return saved;
    inp.close();
}

void Data::Save::loadGame(int flag) {
    std::ifstream inp((flag == PVP_SCREEN) ? "history/savedPVP.game" : "history/savedPvC.game");
    int saved, type; inp >> saved >> type;
    int m, n; inp >> m >> n;
    
    GameState::setTypeGame(type);
    GameState::setStateRow(m);
    GameState::setStateCol(n);

    for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) {
        int v; inp >> v;
        GameState::setStateAt(i, j, v);
    }
    inp.close();
}

void Data::Statis::saveGame(int flag) {
    std::ofstream out;
    out.open((flag == STATISTIC_PVP) ? "history/PvP.statis" : "history/PvC.statis", std::ios::app);
    out << Utils::Times::getDateTime() << " ";
    out << GameState::getStateRow() << " " << GameState::getStateCol() << " ";
    for (int i = 1; i <= GameState::getStateRow(); ++i) for (int j = 1; j <= GameState::getStateCol(); ++j) {
        if (GameState::getStateAt(i, j) == 0) out << "0";
        if (GameState::getStateAt(i, j) == 1) out << "1";
        if (GameState::getStateAt(i, j) == 2) out << "2";
    }
    out << "\n";
    out.close();
}

void Data::Statis::loadStatis(int flag) {
    std::ifstream inp((flag == STATISTIC_PVP) ? "history/PvP.statis" : "history/PvC.statis");
    list.clear();
    if (!inp) return;
    std::string s, t, c; int n, m;
    
    std::vector < std::string > cur;
    while (inp >> s) cur.push_back(s);
    reverse(cur.begin(), cur.end());
    
    for (int i = 0; i < std::min((int) cur.size(), 5 * 5); ++i) list.push_back(cur[i]);
    reverse(list.begin(), list.end());

    inp.close();
}

int Data::Statis::getStatisSize() { return list.size() / 5; }

std::string Data::Statis::getStatisName(int i) {
    return list[5 * i] + " " + list[5 * i + 1];
}

void Data::Statis::getState(int _type, int i) {
    std::string s, t, c; int n, m;
    s = 5 * i;
    t = list[5 * i + 1]; 
    m = to_int(list[5 * i + 2]);
    n = to_int(list[5 * i + 3]);
    c = list[5 * i + 4];
    
    GameState::setBoardSize(m, n);
    GameState::setStateRow(m);
    GameState::setStateCol(n);
    int x = 1, y = 1;
    Graphic::Screens::Clear(0, 0, LINES, COLS);
    for (int i = 0; i < c.size(); ++i) {
        if (c[i] == '0') GameState::setStateAt(x, y, 0);
        if (c[i] == '1') GameState::setStateAt(x, y, 1);
        if (c[i] == '2') GameState::setStateAt(x, y, 2);
        y++;
        if (y > n) {
            x++;
            y = 1;
        }
    }
    GameState::print();
    attron(A_BOLD | A_BLINK);
    mvaddstr(25, 104, "Press B to return!");
    attroff(A_BOLD | A_BLINK);
    refresh();

    while (true) {
        Ui::Input::read();
        if (Ui::Input::is_B_Key()) {
            Graphic::Screens::Clear(0, 0, LINES, COLS);
            Graphic::Screens::sketchMainWindow();
            if (_type == STATISTIC_PVP) Graphic::Screens::sketchStatisPVPScreen();
            else Graphic::Screens::sketchStatisPVCScreen();
            break;
        }
    }
    refresh();
}

int Data::Statis::to_int(std::string s) {
    int x = 0;
    for (int i = 0; i < s.size(); ++i) x = x * 10 + s[i] - '0';
    return x;
}