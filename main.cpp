#include <iostream>
#include "Game.hpp"

int main() {

    int board_size, num_to_win;
    std::cout << "Board size: ";
    std::cin >> board_size;
    std::cout << "Num to win: ";
    std::cin >> num_to_win;

    TicTacToe game(board_size, num_to_win);
    game.gameplay();

    return 0;
}
