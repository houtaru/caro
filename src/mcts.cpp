#include <mcts.h>

#include <cmath>
#include <ctime>
#include <algorithm>

namespace MCTS {
    Node::Node(const Node& p) {
        child = p.child;
        
        moves = p.moves;
        
        wins = p.wins;
        
        visits = p.visits;
        
        move = p.move;
        
        parent = p.parent;
        
        _icon = p._icon;
    }

    Node::Node(const board& b, const coordinate& p, Node * _parent) {
        move = p;
        
        moves = b.getMoves();
        
        _icon = b.getIcon();

        parent = _parent;
    }

    Node& Node::operator = (const Node& p) {
        child = p.getChild();
        
        moves = p.moves;
        
        UCT_score = p.UCT_score;
        
        wins = p.wins;
        
        visits = p.visits;
        
        move = p.move;
        
        parent = p.parent;
        
        _icon = p._icon;
        
        return *this;
    }

    Node::~Node() {
        for (Node* cc : child) delete cc;
    }

    std::vector <Node*> Node::getChild() const { return child; }

    double Node::getUCTscore() const { return UCT_score; }

    double Node::getWins() const { return wins; }

    int Node::getVisits() const { return visits; }

    coordinate Node::getMove() const { return move; }

    Node* Node::getParent() const { return parent; }

    Node* Node::addChild(const coordinate& mv, const board& b) {
        Node* cc = new Node(b, mv, this);
        child.push_back(cc);
        
        for (auto it = moves.begin(); it != moves.end(); ++it) if (*it == mv) {
            moves.erase(it);
            break;
        }
        return cc;
    }

    bool Node::hasChildren() const { return !child.empty(); }

    Node* Node::getUTCchild() const {
        for (Node* it : child) {
            it->UCT_score = double(it->wins) / double(it->visits) + sqrt(2.0 + log(double(visits) / it->visits));
        }
        return *std::max_element(child.begin(), child.end(), [](const Node* p, const Node* q) {
            return p->getUCTscore() < q->getUCTscore();
        });
    }

    bool Node::haveMoves() const { return !moves.empty(); }

    coordinate Node::getExpandedMove() const { return moves[0]; }

    void Node::update(const double x) {
        ++visits;
        wins += x;
    }

    coordinate computeMove(const board& b) {
        std::vector <coordinate> moves = b.getList();
        if (!moves.empty()) return moves[0];

        moves = b.getMoves();
        if (moves.size() == 1) return moves[0];

        Node root = computeTree(b);
        long long played = root.getVisits();

        std::map <coordinate, int> visits;
        std::map <coordinate, double> wins;
        std::vector <Node*> child = root.getChild();

        for (auto it = child.cbegin(); it != child.cend(); ++it) {
            visits[(*it)->getMove()] += (*it)->getVisits();
            wins[(*it)->getMove()] += (*it)->getWins();
        }

        double best = -1;
        coordinate result = coordinate();

        for (auto it : visits) {
            coordinate move = it.first;
            double v = it.second;
            double w = wins[move];

            double rate = (w + 1) / ( + 1);
            if (rate > best) {
                best = rate;
                result = move;
            }
        }
        return result;
    }

    Node computeTree(board b) {
        time_t start = std::clock();
        Node* node = new Node(b);

        while (true) {
            //Selection
            while (!node->haveMoves() && node->hasChildren()) {
                node = node->getUTCchild();
                b.doMove(node->getMove());
            }
            //Expandsion
            if (node->haveMoves()) {
                coordinate move = node->getExpandedMove();
                b.doMove(move);
                node = node->addChild(move, b);
            }
            //Simulation
            while (b.judge() == Gomoku::on) b.simulation();

            //Back-propogation
            Node* node_parent = node->getParent();
            while (node_parent != NULL) {
                node->update(b.judge());
                node = node_parent;
                node_parent = node->getParent();
            }
            //time threshold;
            time_t end = std::clock();
            if ((end - start) * 1000 / CLOCKS_PER_SEC > THERESHOLD_TIME) break;
        }
        return *node;
    }
}