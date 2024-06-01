#ifndef MINMAX_GAME_HPP
#define MINMAX_GAME_HPP

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

const char PLAYER = 'X';
const char COMPUTER = 'O';
const char EMPTY = '*';

class TicTacToe {
private:
    int board_size;
    int num_to_win;
    int max_depth;
    std::vector<std::vector<char>> board;
    sf::RenderWindow window;

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

    void displayBoard(std::optional<char> score = std::nullopt) {
        window.clear(sf::Color::Black);

        sf::Text result_prompt;
        sf::Font font;
        font.loadFromFile("../arial.ttf");

        if (score.has_value()) {
            if (score.value() == PLAYER) {
                result_prompt.setString("Player Wins!");
                result_prompt.setFont(font);
                result_prompt.setPosition(window.getSize().x / 5, window.getSize().y / 2);
                result_prompt.setCharacterSize(75);
                result_prompt.setFillColor(sf::Color::Red);
            }
            else if (score.value() == COMPUTER) {
                result_prompt.setString("Computer Wins!");
                result_prompt.setFont(font);
                result_prompt.setPosition(window.getSize().x / 5, window.getSize().y / 2);
                result_prompt.setCharacterSize(75);
                result_prompt.setFillColor(sf::Color::Blue);
            }
            else {
                result_prompt.setString("Draw!");
                result_prompt.setFont(font);
                result_prompt.setPosition(window.getSize().x / 5, window.getSize().y / 2);
                result_prompt.setCharacterSize(75);
                result_prompt.setFillColor(sf::Color::Green);
            }

        }

        window.draw(result_prompt);

        float cell_width = window.getSize().x / static_cast<float>(board_size);
        float cell_height = window.getSize().y / static_cast<float>(board_size);
        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                sf::RectangleShape cell(sf::Vector2f(cell_width, cell_height));
                cell.setPosition(j * cell_width, i * cell_height);
                cell.setOutlineThickness(2.f);
                cell.setOutlineColor(sf::Color::White);
                cell.setFillColor(sf::Color::Transparent);
                window.draw(cell);

                if (board[i][j] == PLAYER) {
                    sf::RectangleShape xMark(sf::Vector2f(cell_width * std::sqrt(2), 4.f));
                    xMark.setPosition(j * cell_width, i * cell_height);
                    xMark.setFillColor(sf::Color::White);
                    xMark.rotate(45.f);
                    window.draw(xMark);
                    sf::RectangleShape x2Mark(sf::Vector2f(cell_width * std::sqrt(2), 3.f));
                    x2Mark.setPosition(j * cell_width, i * cell_height + cell_height);
                    x2Mark.setFillColor(sf::Color::White);
                    x2Mark.rotate(-45.f);
                    window.draw(x2Mark);
                }
                else if (board[i][j] == COMPUTER) {
                    sf::CircleShape oMark(cell_width / 2);
                    oMark.setPosition(j * cell_width, i * cell_height);
                    oMark.setFillColor(sf::Color::Transparent);
                    oMark.setOutlineThickness(3.f);
                    oMark.setOutlineColor(sf::Color::White);
                    oMark.setOrigin(cell_width / 2, cell_height / 2);
                    oMark.move(cell_width / 2, cell_height / 2);
                    window.draw(oMark);
                }
            }
        }

        window.display();
    }

public:
    TicTacToe(): board_size(3), num_to_win(3), max_depth(100 / 9 + 2), board(board_size, std::vector<char>(board_size, EMPTY)), window(sf::VideoMode(800, 800), "Tic Tac Toe") {}

    explicit TicTacToe(int board_size) {
        this->board_size = board_size;
        this->num_to_win = board_size;
        this->max_depth = 100 / (board_size * board_size) + std::ceil(std::sqrt(board_size));
        board.resize(board_size, std::vector<char>(board_size, EMPTY));
        window.create(sf::VideoMode(800, 800), "Tic Tac Toe");
    }

    explicit TicTacToe(int board_size, int num_to_win ) {
        this->board_size = board_size;
        this->num_to_win = std::min(num_to_win, board_size);
        this->max_depth = 100 / (board_size * board_size) + std::ceil(std::sqrt(board_size));
        if (num_to_win > board_size || num_to_win < 3) {
            std::cout << "Provided num to win is invalid, using board size as num to win" << std::endl;
        }
        board.resize(board_size, std::vector<char>(board_size, EMPTY));
        window.create(sf::VideoMode(800, 800), "Tic Tac Toe");
    }

    void gameplay() {
        bool is_game_over = false;

        while (window.isOpen()) {
            if (!is_game_over) displayBoard();

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();

                else if (event.type == sf::Event::MouseButtonPressed && !is_game_over) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        const int mouse_x = event.mouseButton.x;
                        const int mouse_y = event.mouseButton.y;
                        const int row = mouse_y / (window.getSize().y / board_size);
                        const int col = mouse_x / (window.getSize().x / board_size);

                        if (is_move_valid(row, col)) {
                            board[row][col] = PLAYER;
                            displayBoard();

                            if (is_win(PLAYER)) {
                                std::cout << "Player Wins!" << std::endl;
                                displayBoard(PLAYER);
                                is_game_over = true;
                            }
                            else if (is_draw()) {
                                std::cout << "Draw!" << std::endl;
                                displayBoard('D');
                                is_game_over = true;
                            }

                            std::pair<int,int> move = computer_move();
                            board[move.first][move.second] = COMPUTER;

                            if (is_win(COMPUTER)) {
                                std::cout << "Computer Wins!" << std::endl;
                                displayBoard(COMPUTER);
                                is_game_over = true;
                            }
                            else if (is_draw()) {
                                std::cout << "Draw!" << std::endl;
                                displayBoard('D');
                                is_game_over = true;
                            }
                        }
                    }
                }
            }
        }
    }
};


#endif //MINMAX_GAME_HPP
