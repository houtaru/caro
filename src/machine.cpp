#include <map>
#include <algorithm>
#include <utility>

#include <ui.h>
#include <machine.h>
#include <GameState.h>

using namespace GameState;

std::vector <int> Machine::check(int px, int py, int icon_) {
    const int _x = px, _y = py;
    int ss[8] = {0};
    int l = 0, r = 0, i, j;

    //row
    for (i = _x - 1; i && state[i][_y] == icon_; --i) l++;
    ss[1] = i > 0 && state[i][_y] == 0;
    
    for (i = _x + 1; i <= m && state[i][_y] == icon_; ++i) r++;
    ss[1] += i <= m && state[i][_y] == 0;

    ss[0] = l + r + 1;
    
    //col
    l = 0; r = 0;
    for (i = _y - 1; i && state[_x][i] == icon_; --i) l++;
    ss[3] = i > 0 && state[_x][i] == 0;
    
    for (i = _y + 1; i <= n && state[_x][i] == icon_; ++i) r++;
    ss[3] += i <= n && state[_x][i] == 0;

    ss[2] = l + r + 1;

    //diagonal
    l = 0; r = 0;
    for (i = _x - 1, j = _y - 1; i && j && state[i][j] == icon_; --i, --j) l++;
    ss[5] = i > 0 && j > 0 && state[i][j] == 0;
    
    for (i = _x + 1, j = _y + 1; i <= m && j <= n && state[i][j] == icon_; ++i, ++j) r++;
    ss[5] += i <= m && j <= n && state[i][j] == 0;

    ss[4] = l + r + 1;

    //anti diagonal
    l = 0; r = 0;
    for (i = _x - 1, j = _y + 1; i && j <= n && state[i][j] == icon_; --i, ++j) l++;
    ss[7] = i && j <= n && state[i][j] == 0;

    for (i = _x + 1, j = _y - 1; i <= m && j && state[i][j] == icon_; ++i, --j) r++;
    ss[7] += i <= n && j && state[i][j] == 0;

    ss[6] = l + r + 1;

    //result
    std::vector <int> result(6, 0);
    for (int i = 0; i < 8; i += 2) {
        result[0] += ss[i] >= 5;
        result[1] += ss[i] == 4 && ss[i + 1] == 2;
        result[2] += ss[i] == 4 && ss[i + 1] == 1;
        result[3] += ss[i] == 3 && ss[i + 1] == 2;
        result[4] += ss[i] == 3 && ss[i + 1] == 1;
        /**/result[5] += ss[i] == 2 && ss[i + 1] == 2;
    }
    return result;
}

std::pair <int, int> Machine::getMove() {
    std::map <int, std::pair <int, int>, std::greater<int> > weight;
    std::vector < std::pair <int, int> > lists;

    for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) if (state[i][j] == 0) {
        std::vector <int> self = check(i, j, 2);
        std::vector <int> oppo = check(i, j, 1);

        int a = self[0] * 87654321 + oppo[0] * 7654321 + self[1] * 654321 + oppo[1] * 654321 + self[2] * 54321 + oppo[2] * 321 + self[3] * 4321;
        if (a > 0) {
            int b = oppo[3] * 3 + self[4] * 5 + self[5] * 5 + oppo[4] * 1 + oppo[5] * 4;
            weight[a + b] = std::make_pair(i, j);
        }
    }
    for (auto it : weight) lists.push_back(it.second);

    if (!lists.empty()) return lists[0];

    for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) if (state[i][j] == 0) {
        bool ok = false;
        ok |= i > 1 && state[i - 1][j] != 0;
        ok |= j > 1 && state[i][j - 1] != 0;
        ok |= i < m && state[i + 1][j] != 0;
        ok |= j < n && state[i][j + 1] != 0;
        ok |= i > 1 && j > 1 && state[i - 1][j - 1] != 0;
        ok |= i > 1 && j < n && state[i - 1][j + 1] != 0;
        ok |= i < m && j > 1 && state[i + 1][j - 1] != 0;
        ok |= i < m && j < n && state[i  + 1][j + 1] != 0;

        if (ok == true) lists.push_back(std::make_pair(i, j));
    }
    if (!lists.empty()) return lists[0 + rand() % (lists.size())];

    int x, y;
    do {
        x = 1 + rand() % m;
        y = 1 + rand() % n;
    } while (state[x][y] != 0);
    return std::make_pair(x, y);
}

void Machine::doMove() {
    std::pair <int, int> p = getMove();
    int x = playWindow.top() + 2 * (p.first - 1) + 1;
    int y = playWindow.left() + 2 * (p.second - 1) + 1;

    state[p.first][p.second] = 2;
    
    if (Ui::Controler::makeSound() == 0) beep();   

    Graphic::Color::colorOn(PLAYER_TWO_COLOR);
    mvaddch(x, y, player[1].getIcon());
    Graphic::Color::colorOff(PLAYER_TWO_COLOR);
}