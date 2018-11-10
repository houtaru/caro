#include <graphic.h>
#include <ui.h>
#include <GameState.h>

using namespace std;

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
    attron(A_BOLD);
    Graphic::Color::reverseColorOn(color);
    mvaddch(x << 1, y << 1, chess);
    Graphic::Color::reverseColorOff(color);
    attroff(A_BOLD);
}

void GameState::PlayerState::next() {
    isCurrentPlayer ^= 1;
    PlayerState::printProfile();
}

void GameState::PlayerState::isWinner() { wins++; }

void GameState::PlayerState::isLoser() { loses++; }

void GameState::PlayerState::isDraw() { draws++; }

void GameState::PlayerState::printProfile() {
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

void GameState::setup(int _m, int _n) {
    reset();

    m = _m; n = _n; winner = -1;
    
    type = Graphic::Screens::currentScreen;

    currentPlayer = 1;

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
    player[0].printProfile();

    if (type == PVP_SCREEN) {
        player[1].printProfile();
    } else {
        currentPlayer = 1;
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

void GameState::reset() {
    for (int i = 0; i < 111; ++i) for (int j = 0; j < 111; ++j)
        state[i][j] = 0;
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
    
    currentPtrPosition.first += x << 1; 
    
    currentPtrPosition.first = max(currentPtrPosition.first, playWindow.top() + 1);
    
    currentPtrPosition.first = min(currentPtrPosition.first, playWindow.bottom() - 1);
    
    currentPtrPosition.second += y << 1;
    
    currentPtrPosition.second = max(currentPtrPosition.second, playWindow.left() + 1);
    
    currentPtrPosition.second = min(currentPtrPosition.second, playWindow.right() - 1);

    xx = currentPtrPosition.first / 2;
    
    yy = currentPtrPosition.second / 2;
    
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
    
    if (state[x][y]) return;
    
    state[x][y] = currentPlayer;
    
    GameState::player[currentPlayer - 1].doMove(x, y);
    
    turnsList.push_back({x, y});

    nextTurn();
}

void GameState::nextTurn() {
    player[currentPlayer - 1].next();
    currentPlayer = 3 - currentPlayer;
    player[currentPlayer - 1].next(); 
}

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
        if (winner != -1)
            mvprintw(playWindow.height() / 2 - 1, playWindow.width() / 2 - 18, "%s WIN! PRESS R TO REPLAY!", player[winner - 1].getName().c_str());
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