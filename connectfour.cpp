#include <iostream>
#include <vector> 

const int ROWS = 6;
const int COLS = 7;

std::vector<std::vector<char>> board(ROWS, std::vector<char>(COLS, ' '));

void printBoard() {
    std::cout << "\n";
    for (int row =0; row < ROWS; row++) {
        std::cout << "|";
        for (int col = 0; col < COLS; col++) {
            std::cout << board[row][col] << "|";
        }
        std::cout << "\n";
    }
    std::cout << " 0 1 2 3 4 5 6\n";
}

bool dropDisc(int col, char player) {
    if (col < 0 || col >= COLS || board[0][col] != ' ') {
        return false;
    }
    for (int row = ROWS - 1; row >=0; row--) {
        if (board[row][col]== ' ') {
            board[row][col] =player;
            return true;
        }
    }
    return false; 
}

bool checkDirection(int row, int col, int dRow, int dCol, char player) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        int r = row + i * dRow;
        int c = col + i * dCol;
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c] != player)
            return false;
    }
    return true;
}

bool checkWin(char player) {
    for (int row =0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (
                checkDirection(row, col, 0, 1, player) ||  // Horizontal
                checkDirection(row, col, 1, 0, player) ||  // Vertical
                checkDirection(row, col, 1, 1, player) ||  // Diagonal /
                checkDirection(row, col, 1, 1, player)     // Diagonal 
            ) {
                return true;
            }
        }
    }
    return false;
}

bool isDraw() {
    for (int col = 0; col < COLS; col++) {
        if (board[0][col] == ' ') return false;
    }
    return true;
}

int main() {
    char currentPlayer = 'X';
    while (true) {
        printBoard();
        int col;
        std::cout << "player " << currentPlayer << ", choose column (0-6): ";
        std::cin >> col;

        if (!dropDisc(col, currentPlayer)) {
            std::cout << "Invalid move. Try again.\n";
            continue;
        }

        if (checkWin(currentPlayer)) {
            printBoard();
            std::cout << "Player " << currentPlayer << " wins!\n";
            break;
        }

        if (isDraw()) {
            printBoard();
            std::cout << "in's a draw!\n";
            break;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
    return 0;
}