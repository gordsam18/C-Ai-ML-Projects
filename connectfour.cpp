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
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (
                checkDirection(row, col, 0, 1, player) ||  // Horizontal
                checkDirection(row, col, 1, 0, player) ||  // Vertical
                checkDirection(row, col, 1, 1, player) ||  // Diagonal /
                checkDirection(row, col, -1, 1, player)     // Diagonal 
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

std::vector<int>getValidMoves(const std::vector<std::vector<char>>& board) {
    std::vector<int> validMoves;
    for (int col = 0; col < COLS; col++ ) {
        if (board[0][col] == ' '){
            validMoves.push_back(col);
        }
    }
    return validMoves;
}

std::vector<std::vector<char>> makeMove(
    const std::vector<std::vector<char>>& board,
    int col,
    char player
) {
    std::vector<std::vector<char>> newBoard = board;
    for (int row = ROWS - 1; row >= 0; row--) {
        if (newBoard[row][col] == ' '){
            newBoard[row][col] = player; 
            break; 
        }
    }
    return newBoard;
}

int evaluateWindow(const std::vector<char>& window) {
    int countAI = 0;
    int countHuman = 0;
    int countEmpty = 0;

    for (char cell: window) {
        if (cell == 'X') {
            countAI++;
        }
        else if (cell == 'O') {
            countHuman++;
        }
        else if (cell == ' ') { 
            countEmpty++;
        }
    }

    if (countAI == 4) {
        return 100000;
    } else if (countAI == 3 && countEmpty == 1) {
        return 500;
    } else if (countAI == 2 && countEmpty == 2) {
        return 50;
    }

    if (countHuman == 4) {
        return -100000;
    } else if (countHuman == 3 && countEmpty == 1) {
        return -70;
    } else if (countHuman == 2 && countEmpty == 2) {
        return -7;
    }
    return 0;
}

int evaluateBoard(const std::vector<std::vector<char>>& board) {
    int score = 0;
    char AI = 'X';
    char Human = 'O';

    int centerCol = COLS/2;
    int centerHeight = 2;


    // center col
    for (int row = 0; row < ROWS; row++) {
        if (board[row][centerCol] == AI) {
            score += centerHeight;
        } else if (board[row][centerCol] == Human) {
            score -= centerHeight;
        }
    }
    // horizontal 
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS - 4; col++) {
            std::vector<char> window;
            for ( int i = 0; i < 4; i++) {
                window.push_back(board[row][col + i]);
            }
            score += evaluateWindow(window);
        }
    }
    // vertical
    for (int row = 0; row < ROWS - 3; row++) {
        for (int col = 0; col < COLS; col++) {
            std::vector<char> window;
            for (int i = 0; i < 4; i++) {
                window.push_back(board[row + i][col]);
            }
            score += evaluateWindow(window);
        }
    }
    // diagonal down
    for (int row = 0; row < ROWS - 4; row++) {
        for (int col = 0; col < COLS - 4; col++) {
            std::vector<char> window;
            for (int i = 0; i < 4; i++) {
                window.push_back(board[row + i][col + i]);
            }
            score += evaluateWindow(window);
        }
    }

    // diagonal up
    for (int row = 3; row < ROWS; row++) {
        for (int col = 0; col < COLS - 4; col++) {
            std::vector<char> window;
            for (int i = 0; i < 4; i++) {
                window.push_back(board[row - i][col + i]);
            }
            score += evaluateWindow(window);
        }
    }
    return score; 
}



int minimax (
    std::vector<std::vector<char>>& board,
    int depth,
    bool maximingPlayer,
    int alpha,
    int beta
) {
    char aiPlayer = 'X';
    char humanPlayer = 'O';


    if (checkWin(aiPlayer)) return 100000 - depth; 
    if (checkWin(humanPlayer)) return -100000 + depth;
    if (isDraw() || depth == 0) return evaluateBoard(board);

    if (maximingPlayer) {
        int maxEval = -1000000;
        for (int col: getValidMoves(board)) {
            auto nextBoard = makeMove(board, col, aiPlayer);
            int eval = minimax(nextBoard, depth - 1, false , alpha, beta);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break; 
        }
        return maxEval;

    }
    else {
        int minEval = 1000000;
        for (int col : getValidMoves(board)) {
            auto nextBoard = makeMove(board, col, humanPlayer);
            int eval = minimax(nextBoard, depth -1, true, alpha, beta);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

int findBestMove(std::vector<std::vector<char>>&board, int depth) {
    int bestScore = -1000000;
    int bestCol = -1;
    std::vector<int> bestCols;

    for (int col : getValidMoves(board)) {
        auto nextBoard = makeMove(board, col, 'X');
        int score = minimax(nextBoard, depth -1, false, -1000000, 1000000);
        std::cout << "Column " << col << " Score: "<< score << std::endl;
        if (score > bestScore) {
            bestScore = score;
            bestCols.clear();
            bestCols.push_back(col);
        } else if (score == bestScore) {
            bestCols.push_back(col);
        }
    }
    if (!bestCols.empty()) {
        bestCol = bestCols[rand() % bestCols.size()];
    }
    std::cout << "Best Column: " << bestCol << " with score: " << bestScore << std::endl;
    return bestCol;
}

int main() {
    char currentPlayer = 'X';
    while (true) {
        printBoard();
        int col;
        std::cout << "player " << currentPlayer << ", choose column (0-6): ";
        if ( currentPlayer == 'X') {
            col = findBestMove(board, 5);
            std::cout << col << std::endl;
        }
        else {
            std::cin >> col;
        }
        

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