#include "sudoku.h"
#include <unordered_set>
#include <vector>
#include <random>
#include <algorithm>


Sudoku::Sudoku() {}; // ctor

Sudoku::~Sudoku() {}; // dtor

void Sudoku::setBoardEmpty() {
    for (auto& row : board) {
        row.fill(0); // Fill each inner array with 0
    }
}

void Sudoku::modifyBoard(const int row, const int col, const int value) {
    board[row][col] = value;
}

int Sudoku::getValue(const int row, const int col) {
    return board[row][col];
}

// Function to check if a value is valid in a given row
bool Sudoku::isRowValid(const int row,const int value) {
    for (int col = 0; col < 9; ++col) {
        if (board[row][col] == value) return false;
    }
    return true;
}

// Function to check if a value is valid in a given column
bool Sudoku::isColumnValid(const int col, const int value) {
    for (int row = 0; row < 9; ++row) {
        if (board[row][col] == value) return false;
    }
    return true;
}

// Function to check if a value is valid in the 3x3 sub-grid
bool Sudoku::isSubtableValid(const int row, const int col, const int value) {
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;

    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j) {
            if (board[i][j] == value) return false;
        }
    }
    return true;
}

// Function to check if a move is valid
bool Sudoku::isMoveValid(const int row, const int col, const int value) {
    if (value == 0) return false;
    return isRowValid(row, value) && isColumnValid(col, value) && isSubtableValid(row, col, value);
}

// Function to solve the Sudoku using backtracking
bool Sudoku::solveSudoku(const int currentRow, const int currentCol) {
    if (currentRow == 9) return true; // Board is solved
    if (currentCol == 9) return solveSudoku(currentRow + 1, 0); // Move to the next row
    if (board[currentRow][currentCol] != 0) return solveSudoku(currentRow, currentCol + 1); // Move to the next column

    for (int value = 1; value <= 9; ++value) {
        if (isMoveValid(currentRow, currentCol, value)) {
            board[currentRow][currentCol] = value;
            if (solveSudoku(currentRow, currentCol + 1)) return true; // Continue with the next cell
            board[currentRow][currentCol] = 0; // Backtrack
        }
    }
    return false;
}

// checks the board if the solution is valid
bool Sudoku::isSolutionCorrect() {
    // Check rows
    for (int row = 0; row < 9; ++row) {
        std::unordered_set<int> seen;
        for (int col = 0; col < 9; ++col) {
            int value = board[row][col];
            if (value < 1 || value > 9 || !seen.insert(value).second) return false;
        }
    }

    // Check columns
    for (int col = 0; col < 9; ++col) {
        std::unordered_set<int> seen;
        for (int row = 0; row < 9; ++row) {
            int value = board[row][col];
            if (value < 1 || value > 9 || !seen.insert(value).second) return false;
        }
    }

    // Check 3x3 sub-grids
    for (int boxRow = 0; boxRow < 9; boxRow += 3) {
        for (int boxCol = 0; boxCol < 9; boxCol += 3) {
            std::unordered_set<int> seen;
            for (int row = boxRow; row < boxRow + 3; ++row) {
                for (int col = boxCol; col < boxCol + 3; ++col) {
                    int value = board[row][col];
                    if (value < 1 || value > 9 || !seen.insert(value).second) return false;
                }
            }
        }
    }
    return true;
}

// Function to generate a random Sudoku solution
bool Sudoku::generateRandomSolution() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                std::shuffle(numbers.begin(), numbers.end(), std::mt19937(std::random_device()()));

                for (int value : numbers) {
                    if (isMoveValid(row, col, value)) {
                        board[row][col] = value;

                        if (generateRandomSolution()) {
                            return true;
                        }

                        board[row][col] = 0; // Backtrack
                    }
                }

                return false; // Trigger backtracking
            }
        }
    }
    return true; // Solution found
}

// Function to generate a Sudoku puzzle based on difficulty
void Sudoku::generateSudokuPuzzle() {
    setBoardEmpty();
    // Generate a complete Sudoku solution
    generateRandomSolution();

    int cellsToRemove;
    switch (difficulty) {
    case EASY:   cellsToRemove = 20; break;
    case MEDIUM: cellsToRemove = 40; break;
    case HARD:   cellsToRemove = 60; break;
    default:     cellsToRemove = 40; break; // MEDIUM
    }

    // Create a list of all cell positions
    std::vector<std::pair<int, int>> cellPositions;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cellPositions.push_back({i, j});
        }
    }

    // Shuffle cell positions to randomize removal
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cellPositions.begin(), cellPositions.end(), g);

    // Remove cells
    for (int i = 0; i < cellsToRemove; ++i) {
        int row = cellPositions[i].first;
        int col = cellPositions[i].second;
        board[row][col] = 0;
    }
}
