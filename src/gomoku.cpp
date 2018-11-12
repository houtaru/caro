#include <gomoku.h>

#include <map>

namespace Gomoku {
    Coordinate::Coordinate(const Coordinate &p) { x = p.getX(); y = p.getY(); }
    
    Coordinate::Coordinate(const int& _x, const int& _y) {x = _x; y = _y; }

    int Coordinate::getX() const { return x; }

    int Coordinate::getY() const { return y; }

    Coordinate& Coordinate::operator = (const Coordinate& p) {
        x = p.getX(); y = p.getY();
        return *this;
    }

    bool Coordinate::operator < (const Coordinate &p) const {
        return x < p.getX() || (x == p.getX() && y < p.getY());
    }

    bool Coordinate::operator != (const Coordinate &p) const {
        return x != p.getX() || y != p.getY();
    }

    bool Coordinate::operator == (const Coordinate &p) const {
        return x == p.getX() && y == p.getY();
    }

    Board::Board(const icon c, const Coordinate p) {
        for (int i = 0; i < 111; ++i) for (int j = 0; j < 111; ++j)
            board[i][j] = null;
        
        _icon = c;
        _pos = p;
    }

    icon Board::getIconAt(const Coordinate &p) const { return board[p.getX()][p.getY()]; }

    icon Board::getIcon() const { return _icon; }

    state Board::judge() const {
        const int _x = _pos.getX(), _y = _pos.getY();
        const icon icon_ = board[_x][_y];

        if (icon_ == null) return on;

        std::vector <int> cc = getLine(_pos, icon_);
        if (cc[0] > 0) return isWin(icon_);

        for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) if (board[i][j] == null);
            return on;
    }

    state Board::isWin(const icon& c) const { return c == _icon ? win : lose; }

    void Board::doMove(const Coordinate& mv) {
        board[mv.getX()][mv.getY()] = _icon;
        _icon = _icon == X ? O : X;
        _pos = mv;
    }

    std::vector <Coordinate> Board::getList() const {
        std::vector <Coordinate> moves;
        std::map <int, Coordinate> weight;
        const icon _opp = _icon == X ? O : X;

        if (board[_pos.getX()][_pos.getY()] == null) moves.push_back(_pos);

        for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) {
            if (board[i][j] != null) continue;
            std::vector <int> self = getLine(Coordinate(i, j), _icon);
            std::vector <int> oppo = getLine(Coordinate(i, j), _opp);

            int exp = self[0] * 87654321 + oppo[0] * 7654321 + self[1] * 654321 + oppo[1] * 654321 + self[2] * 54321 + oppo[2] * 321 + self[3] * 4321;
            if (exp > 0) {
                int exr = oppo[3] * 3 + self[4] * 5 + self[5] * 5 + oppo[4] * 1 + oppo[5] * 4;
                weight[exp + exr] = Coordinate(i, j);
            }
        }
        /**/for (auto it = weight.rbegin(); it != weight.rend(); ++it)
                moves.push_back(it->second);
        return moves;
    }

    std::vector <Coordinate> Board::getMoves() const {
        std::vector <Coordinate> moves;
        if (board[_pos.getX()][_pos.getY()] == null) moves.push_back(_pos);

        for (int i = 1; i <= m; ++i) for (int j = 1; j <= n; ++j) {
            if (board[i][j] != null) continue;

            bool ok = false;
            ok |= i > 1 && board[i - 1][j] != null;
            ok |= j > 1 && board[i][j - 1] != null;
            ok |= i < m && board[i + 1][j] != null;
            ok |= j < n && board[i][j + 1] != null;
            ok |= i > 1 && j > 1 && board[i - 1][j - 1] != null;
            ok |= i > 1 && j < n && board[i - 1][j + 1] != null;
            ok |= i < m && j > 1 && board[i + 1][j - 1] != null;
            ok |= i < m && j < n && board[i  + 1][j + 1] != null;

            if (ok == true) moves.push_back(Coordinate(i, j));
        }
        return moves;
    }

    void Board::simulation() {
        doMove(getMoves()[0]);
    }

    std::vector <int> Board::getLine(const Coordinate &p, const icon & icon_) const {
        const int _x = p.getX(), _y = p.getY();
        int ss[8] = {0};
        int l = 0, r = 0, i, j;

        //row
        for (i = _x - 1; i && board[i][_y] == icon_; --i) l++;
        ss[1] = i > 0 && board[i][_y] == null;
        
        for (i = _x + 1; i <= m && board[i][_y] == icon_; ++i) r++;
        ss[1] += i <= m && board[i][_y] == null;

        ss[0] = l + r + 1;
        
        //col
        l = 0; r = 0;
        for (i = _y - 1; i && board[_x][i] == icon_; --i) l++;
        ss[3] = i > 0 && board[_x][i] == null;
        
        for (i = _y + 1; i <= n && board[_x][i] == icon_; ++i) r++;
        ss[3] += i <= n && board[_x][i] == null;

        ss[2] = l + r + 1;

        //diagonal
        l = 0; r = 0;
        for (i = _x - 1, j = _y - 1; i && j && board[i][j] == icon_; --i, --j) l++;
        ss[5] = i > 0 && j > 0 && board[i][j] == null;
        
        for (i = _x + 1, j = _y + 1; i && j && board[i][j] == icon_; ++i, ++j) r++;
        ss[5] += i <= m && j <= n && board[i][j] == null;

        ss[4] = l + r + 1;

        //anti diagonal
        l = 0; r = 0;
        for (i = _x - 1, j = _y + 1; i && j <= n && board[i][j] == icon_; --i, ++j) l++;
        ss[7] = i && j <= n && board[i][j] == null;

        for (i = _x + 1, j = _y - 1; i <= m && j && board[i][j] == icon_; ++i, --j) r++;
        ss[7] += i <= n && j && board[i][j] == null;

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
};