#include "Board.hpp"

int main(int argc, char** argv) {
    int n = 11; 
    if (argc >= 2) {
        n = std::stoi(argv[1]);
    }

    Board board(n);

    for (char c; c != EOF && !board.ended(); c = getchar()) {
    // for (;;) {
        Index next = board.select();
        board.add_as_thunder(next);
        board.print();
    }
}
