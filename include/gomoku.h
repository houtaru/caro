#ifndef GOMOKU_H_DEFINED
#define GOMOKU_H_DEFINED

#include <utility>
#include <vector>

namespace Gomoku {
    enum icon {X, O, null};

    enum state {win, lose, draw, on};

    class Coordinate {
        public:
            Coordinate() {}

            Coordinate(const Coordinate&);

            Coordinate(const int&, const int&);

            Coordinate& operator = (const Coordinate&);

            ~Coordinate() {}

            int getX() const;

            int getY() const;

            bool operator < (const Coordinate&) const;

            bool operator == (const Coordinate&) const;

            bool operator != (const Coordinate&) const;
        private:
            int x, y;
    };

    class Board {
        public:
            Board(const icon = null, const Coordinate = Coordinate(-1, -1));

            icon getIconAt(const Coordinate&) const;
            
            icon getIcon() const;

            state judge() const;

            void doMove(const Coordinate&);

            std::vector <Coordinate> getList() const;

            std::vector <Coordinate> getMoves() const;

            void simulation();
        private:
            int m, n;
            icon board[111][111], _icon;
            Coordinate _pos;

            std::vector <int> getLine(const Coordinate&, const icon&) const;

            state isWin(const icon&) const;
    };
    static const Coordinate notFound(-1, -1);
};

#endif