#include <data.h>
#include <utils.h>
#include <graphic.h>
#include <GameState.h>

#include <fstream>

std::vector < std::tuple <std::string, std::string, int, int, std::string> > Data::Statis::list;

void Data::Save::savedGame(bool flag) {
    int currentScreen = Graphic::Screens::getCurrentScreen();
    std::ofstream out((currentScreen == PVP_SCREEN) ? "history/savedPVP.game" : "history/savedPvC.game");

    out << flag << "\n";
    
    out << currentScreen << "\n";
    
    out << GameState::getStateRow() << " " << GameState::getStateCol() << "\n";

    for (int i = 1; i <= GameState::getStateRow(); ++i)
        for (int j = 1; j <= GameState::getStateCol(); ++j)
            out << GameState::getStateAt(i, j) << (j == GameState::getStateCol() ? "\n" : " ");
    
    out.close();
}

bool Data::Save::canLoadGame(int flag) {
    std::ifstream inp((flag == PVP_SCREEN) ? "history/savedPVP.game" : "history/savedPvC.game");
    int saved; inp >> saved;
    return saved;
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
    out << GameState::getBoardHeight() << " " << GameState::getBoardWidth() << " ";
    for (int i = 1; i <= GameState::getStateRow(); ++i) for (int j = 1; j <= GameState::getStateCol(); ++j) {
        if (GameState::getStateAt(i, j) == 0) out << "00";
        if (GameState::getStateAt(i, j) == 1) out << "01";
        if (GameState::getStateAt(i, j) == 2) out << "10";
    }
    out.close();
}

void Data::Statis::loadStatis(int flag) {
    std::ifstream inp((flag == STATISTIC_PVP) ? "history/PvP.statis" : "history/PvC.statis");
    std::string s, t, c; int n, m;
    while (inp >> s >> t >> c >> m >> n) {
        list.emplace_back(s, t, m, n, c);
    }
    inp.close();
}

int Data::Statis::getStatisSize() { return list.size(); }

std::string Data::Statis::getStatisName(int i) {
    std::string s, t, c; int n, m;
    tie(s, t, m, n, c)  = list[i];
    return s + " " + t;
}

void Data::Statis::getState(int i) {
    std::string s, t, c; int n, m;
    tie(s, t, m, n, c)  = list[i];
    
    GameState::setBoardSize(2 * m + 1, 2 * n + 1);
    GameState::setStateRow(m);
    GameState::setStateCol(n);
    int x = 1, y = 1;
    for (int i = 0; i < c.size(); i += 2) {
        if (c[i] == '0' && c[i + 1] == '0') GameState::setStateAt(x, y, 0);
        if (c[i] == '0' && c[i + 1] == '1') GameState::setStateAt(x, y, 1);
        if (c[i] == '1' && c[i + 1] == '0') GameState::setStateAt(x, y, 2);
        y++;
        if (y > n) x++; y = 1;
    }
    GameState::print();
}