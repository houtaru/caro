#include <data.h>
#include <GameState.h>

#include <fstream>
#include <graphic.h>

void Data::savedGame(bool flag) {
    int currentScreen = Graphic::Screens::getCurrentScreen();
    std::ofstream out((currentScreen == PVP_SCREEN) ? "savedPVP.game" : "savedPvC.game");

    out << flag << "\n";
    
    out << currentScreen << "\n";
    
    out << GameState::getStateRow() << " " << GameState::getStateCol() << "\n";

    for (int i = 1; i <= GameState::getStateRow(); ++i)
        for (int j = 1; j <= GameState::getStateCol(); ++j)
            out << GameState::getStateAt(i, j) << (j == GameState::getStateCol() ? "\n" : " ");
    
    out.close();
}

bool Data::canLoadGame(int flag) {
    std::ifstream inp((flag == PVP_SCREEN) ? "savedPVP.game" : "savedPvC.game");
    int saved; inp >> saved;
    return saved;
}

void loadGame(int flag) {
    std::ifstream inp((flag == PVP_SCREEN) ? "savedPVP.game" : "savedPvC.game");
    int saved, type; inp >> saved >> type;
    int m, n; inp >> m >> n;
    
    GameState::setTypeGame(type);
    GameState::setStateRow(m);
    GameState::setStateCol(n);

    for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) {
        int v; inp >> v;
        GameState::setStateAt(i, j, v);
    }
}

void Data::loadStatis(int flag) {
    //std::ifstream inp((flag == STATISTIC_PVP))
}