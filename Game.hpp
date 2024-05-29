#ifndef MINMAX_GAME_HPP
#define MINMAX_GAME_HPP

#include <vector>
#include <iostream>

const char PLAYER = 'X';
const char COMPUTER = 'O';
const char EMPTY = '*';

const char DRAW = 'D';
const char NONE = 'N';

class TicTacToe {
private:
    int board_size;
    std::vector<std::vector<char>> board;

    char checkWin() const {
        // check rows
        for (int i = 0; i < board_size; i++) {
            bool whole_row = true;
            for (int j = 0; j < board_size - 1; j++) {
                if (board[i][j] != board[i][j + 1]) {
                    whole_row = false;
                    break;
                }
            }
            if (whole_row && board[i][0] != EMPTY) return board[i][0];
        }

        // check columns
        for (int i = 0; i < board_size - 1; i++) {
            bool whole_column = true;
            for (int j = 0; j < board_size; j++) {
                if (board[j][i] != board[j][i + 1]) {
                    whole_column = false;
                    break;
                }
            }
            if (whole_column && board[0][i] != EMPTY) return board[0][i];
        }

        // check diagonals
        bool whole_diagonal = true;
        for (int i = 0; i < board_size - 1; i++) {
            if (board[i][i] != board[i + 1][i + 1]) whole_diagonal = false;
        }
        if (whole_diagonal && board[0][0] != EMPTY) return board[0][0];

        whole_diagonal = true;
        for (int i = 0 ; i < board_size - 1; i++) {
            if (board[i][board_size - i - 1] != board[i + 1][board_size - i - 2]) whole_diagonal = false;
        }
        if (whole_diagonal && board[0][board_size - 1] != EMPTY) return board[0][board_size - 1];

        // check draw
        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                if (board[i][j] == EMPTY) return NONE;
            }
        }

        return DRAW;
    }

    void reset() {
        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                board[i][j] = EMPTY;
            }
        }
    }

    bool is_move_valid(int row, int col) const {
        const bool is_inside_board = row >= 0 && row < board_size && col >= 0 && col < board_size;
        if (!is_inside_board) return false;

        const bool is_empty_cell = board[row][col] == EMPTY;
        return is_empty_cell;
    }

public:
    TicTacToe(): board_size(3), board(board_size, std::vector<char>(board_size, EMPTY)) {}

    explicit TicTacToe(int board_size ) {
        this->board_size = board_size;
        board.resize(board_size, std::vector<char>(board_size, EMPTY));
    }

    void displayBoard() const {
        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                std::cout << board[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void gameplay() {
        char current_player = PLAYER;

        while (true) {
            displayBoard();
            std::cout << "Player " << current_player << " turn" << std::endl;
            std::cout << std::endl;

            int row, col;
            std::cin >> row >> col;

            if (!is_move_valid(row, col)) {
                std::cout << "Invalid move" << std::endl;
                continue;
            }

            board[row][col] = current_player;

            const char result = checkWin();
            if (result != NONE) {
                if (result == DRAW) {
                    std::cout << "It's a draw!" << std::endl;
                    break;
                }

                std::cout << "Player " << result << " won!" << std::endl;
                break;
            }

            current_player = current_player == PLAYER ? COMPUTER : PLAYER;
        }

        displayBoard();
        std::cout << "Game over!" << std::endl;
    }


    // void playerMove();
    // void computerMove();
    // bool isGameOver();
    // bool isDraw();
    // bool isWin(char player);
    // void play();
    // void reset();

};


#endif //MINMAX_GAME_HPP
