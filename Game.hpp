#ifndef MINMAX_GAME_HPP
#define MINMAX_GAME_HPP

#include <vector>
#include <iostream>


const char PLAYER = 'X';
const char COMPUTER = 'O';
const char EMPTY = '*';


class TicTacToe {
private:
    int board_size;
    int num_to_win;
    int max_depth;
    std::vector<std::vector<char>> board;

    bool is_win(char player) const {
        // check rows
        for (int i = 0; i < board_size; i++) {
            int row_count = 0;
            for (int j = 0; j < board_size; j++) {
                if (board[i][j] == player) {
                    row_count++;
                    if (row_count >= num_to_win) return true;
                }
                else if (board[i][j] != player) row_count = 0;
            }
        }

        // check columns
        for (int i = 0; i < board_size; i++) {
            int col_count = 0;
            for (int j = 0; j < board_size; j++) {
                if (board[j][i] == player) {
                    col_count++;
                    if (col_count >= num_to_win) return true;
                }
                else if (board[i][j] != player) col_count = 0;
            }
        }

        // check diagonals
        int left_to_right_diag_count = 0;
        for (int i = 0; i < board_size; i++) {
            if (board[i][i] == player) {
                left_to_right_diag_count++;
                if (left_to_right_diag_count >= num_to_win) return true;
            }
            else if (board[i][i] != player) left_to_right_diag_count = 0;

        }

        int right_to_left_diag_count = 0;
        for (int i = 0 ; i < board_size; i++) {
            if (board[i][board_size - i - 1] == player) {
                right_to_left_diag_count++;
                if (right_to_left_diag_count >= num_to_win) return true;
            }
            else if (board[i][board_size - i - 1] != player) right_to_left_diag_count = 0;
        }

        return false;
    }

    bool is_draw() const {
        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                if (board[i][j] == EMPTY) return false;
            }
        }
        return true;
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

    std::pair<int, int> computer_move() {
        int best_score = INT_MIN;
        std::pair<int, int> best_move;

        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                if (!is_move_valid(i, j)) continue;

                board[i][j] = COMPUTER;
                int score = min_max_search(false, 1, INT_MIN, INT_MAX);
                board[i][j] = EMPTY;

                if (score > best_score) {
                    best_score = score;
                    best_move = std::make_pair(i, j);
                }
            }
        }

        return best_move;
    }

    int min_max_search(bool maximizing, int depth, int alpha, int beta) {
        if (is_win(COMPUTER)) return 1 * (1 / depth);
        if (is_win(PLAYER)) return -1 * depth;
        if (is_draw()) return 0;

        if (depth == max_depth) return 0;

        if (maximizing) {
            int best_score = INT_MIN;
            for (int i = 0; i < board_size; i++) {
                for (int j = 0; j < board_size; j++) {
                    if (!is_move_valid(i, j)) continue;

                    board[i][j] = COMPUTER;
                    int score = min_max_search(false, depth + 1, alpha, beta);
                    board[i][j] = EMPTY;

                    best_score = std::max(best_score, score);
                    alpha = std::max(alpha, score);
                    if (alpha >= beta) break;
                }
            }

            return best_score;
        }

        int best_score = INT_MAX;
        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                if (!is_move_valid(i, j)) continue;

                board[i][j] = PLAYER;
                int score = min_max_search(true, depth + 1, alpha, beta);
                board[i][j] = EMPTY;

                best_score = std::min(best_score, score);
                beta = std::min(beta, score);
                if (alpha >= beta) break;
            }
        }

        return best_score;
    }

    void displayBoard() const {
        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                std::cout << board[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

public:
    TicTacToe(): board_size(3), num_to_win(3), max_depth(100 / 9 + 2), board(board_size, std::vector<char>(board_size, EMPTY)) {}

    explicit TicTacToe(int board_size) {
        this->board_size = board_size;
        this->num_to_win = board_size;
        this->max_depth = 100 / (board_size * board_size) + std::ceil(std::sqrt(board_size));
        board.resize(board_size, std::vector<char>(board_size, EMPTY));
    }

    explicit TicTacToe(int board_size, int num_to_win ) {
        this->board_size = board_size;
        this->num_to_win = std::min(num_to_win, board_size);
        this->max_depth = 100 / (board_size * board_size) + std::ceil(std::sqrt(board_size));
        if (num_to_win > board_size) {
            std::cout << "Provided num to win is greater than board size, using board size as num to win" << std::endl;
        }
        board.resize(board_size, std::vector<char>(board_size, EMPTY));
    }

    void gameplay() {
        char current_player = PLAYER;

        while (true) {
            displayBoard();
            std::cout << "Player " << current_player << " turn" << std::endl;

            std::pair<int, int> move;
            if (current_player == COMPUTER) {
                move = computer_move();
            } else {
                int row, col;
                std::cin >> row >> col;

                if (!is_move_valid(row, col)) {
                    std::cout << "Invalid move" << std::endl;
                    continue;
                }

                move = std::make_pair(row, col);
            }

            board[move.first][move.second] = current_player;

            if (is_win(current_player)) {
                std::cout << "Player " << current_player << " won!" << std::endl;
                break;
            }

            if (is_draw()) {
                std::cout << "It's a draw!" << std::endl;
                break;
            }

            current_player = current_player == PLAYER ? COMPUTER : PLAYER;
        }

        displayBoard();
        std::cout << "Game over!" << std::endl;
    }
};


#endif //MINMAX_GAME_HPP
