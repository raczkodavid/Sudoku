#include "solver.h"
#include <unordered_set>

Solver::Solver() {}; // ctor

Solver::~Solver() {}; // dtor

bool Solver::isRowValid(int board[9][9], const int row, int const value) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == value) return false;
    }
    return true;
}

bool Solver::isColumnValid(int board[9][9], const int col, const int value) {
    for (int i = 0; i < 9; ++i) {
        if (board[i][col] == value) return false;
    }
    return true;
}

bool Solver::isSubtableValid(int board[9][9], const int row, const int col, const int value) {
    // this two represents the top left corner of the subtable
    const int startRow = (row / 3) * 3;
    const int startCol = (col / 3) * 3;

    // iterate through the subtable to check for existing target value
    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j) {
            if (board[i][j] == value) return false;
        }
    }
    return true;
}

bool Solver::isMoveValid(int board[9][9], const int row, const int col, const int value) {
    if (value == 0) return false;
    return isRowValid(board, row, value) && isColumnValid(board, col, value) && isSubtableValid(board, row, col, value);
}

bool Solver::solveSudoku(int board[9][9], const int currentRow, const int currentCol) {
    if (currentRow == 9) return true; // we reached the end of the board
    if (currentCol == 9) return solveSudoku(board, currentRow + 1, 0); // we reached the end of the current row, we step over
    if (board[currentRow][currentCol] != 0) return solveSudoku(board, currentRow, currentCol + 1); // already filled in

    for (int v = 1; v <= 9; ++v) {
        if (isMoveValid(board, currentRow, currentCol, v)) {
            board[currentRow][currentCol] = v;
            if (solveSudoku(board, currentRow, currentCol + 1)) return true; // if there is a valid solution with this config, then we solved the problem
            board[currentRow][currentCol] = 0; // reset cell to 0
        }
    }
    return false;
}

bool Solver::isSolutionCorrect(int board[9][9]) {
    // check rows
    int value;
    for (int i = 0; i < 9; ++i) {
        std::unordered_set<int> seen;
        for (int j = 0; j < 9; ++j) {
            value = board[i][j];
            if (value < 1 || value > 9 || !seen.insert(value).second) return false;
        }
    }

    // check cols
    for (int j = 0; j < 9; ++j) {
        std::unordered_set<int> seen;
        for (int i = 0; i < 9; ++i) {
            value = board[i][j];
            if (value < 1 || value > 9 || !seen.insert(value).second) return false;
        }
    }

    // check subtables
    for (int boxRow = 0; boxRow < 9; boxRow += 3) {
        for (int boxCol = 0; boxCol < 9; boxCol += 3) {
            std::unordered_set<int> seen;
            for (int row = boxRow; row < boxRow + 3; ++row) {
                for (int col = boxCol; col < boxCol + 3; ++col) {
                    value = board[row][col];
                    if (value < 1 || value > 9 || !seen.insert(value).second) return false;
                }
            }
        }
    }
    return true;
}







