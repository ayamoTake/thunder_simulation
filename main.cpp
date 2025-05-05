#include "Board.hpp"
#include <cstring>
#include <chrono>
#include <thread>

void do_live(Board& board, bool debug_mood) {
    bool is_reached = false;
    while(!is_reached) {
        Index next = board.select();
        is_reached = board.add_as_thunder(next);
        if (debug_mood) {
            board.d_print();
        } else {
            board.print();
        }
        this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(int argc, char** argv) {
    int n = 11; 

    bool debug_mood = false;
    bool live_mood = false;
    double eta = 5;
    int seed = 0;

    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-d") == 0) {
            debug_mood = true;
        } else if (std::strcmp(argv[i], "-l") == 0) {
            live_mood = true;
        } else if (std::strcmp(argv[i], "-e") == 0) {
            eta = stof(argv[++i]);
        } else if (std::strcmp(argv[i], "-s") == 0) {
            seed = stoi(argv[++i]);
        } else {
            n = std::stoi(argv[i]);
        }
    }

    Board board(n, eta, seed);

    if (debug_mood)
        // board.d_print();
        board.print();
    else 
        board.print();

    if (live_mood) {
        do_live(board, debug_mood);
        return 0;
    }

    bool is_reached = false;
    for (char c; c != EOF && !board.ended() && !is_reached; c = getchar()) {
        Index next = board.select();
        is_reached = board.add_as_thunder(next);
        if (debug_mood)
            board.d_print();
        else 
            board.print();
    }

}
