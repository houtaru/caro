#include <gomoku.h>
#include <machine.h>
#include <GameState.h>

using namespace Gomoku;

namespace Machine {
    int level;

    Board board(X, Coordinate(GameState::getPosOx(), GameState::getPosOy()));

    void setLevel(int x) { level = x; }

    void easy() {}

    void medium() {}

    void hard() {
        Coordinate _move;
        state _judge;

        while (!GameState::haveWinner()) {
            if (board.getIcon() == O) {
                move 
            }
        }
    }
}