#ifndef MCTS_H_DEFINED
#define MCTS_H_DEFINED

#include <map>
#include <chrono>

#include <gomoku.h>

namespace MCTS {
    typedef typename Gomoku::Board board;
    typedef typename Gomoku::Coordinate coordinate;
    typedef typename Gomoku::icon icon;

    static const double THERESHOLD_TIME = 10000;

    class Node {
        public:
            Node(){}

            Node(const Node&);
            
            Node(const board&, const coordinate& = Gomoku::notFound, Node* = NULL);

            Node& operator = (const Node&);

            ~Node();

            std::vector <Node*> getChild() const;

            double getUCTscore() const;

            double getWins() const;

            int getVisits() const;

            coordinate getMove() const;

            Node* getParent() const;

            Node* addChild(const coordinate&, const board&);

            bool hasChildren() const;

            Node* getUTCchild() const;

            bool haveMoves() const;

            coordinate getExpandedMove() const;

            void update(const double);
        private:
            std::vector <Node*> child;
            std::vector <coordinate> moves;

            double UCT_score = 0;
            double wins = 0;
            int visits = 0;

            coordinate move;
            Node* parent;
            icon _icon;
    };

    coordinate computeMove(const board&);

    Node computeTree(board);
};

#endif