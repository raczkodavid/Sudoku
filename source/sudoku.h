#ifndef SUDOKU_H
#define SUDOKU_H

#include <array>

enum DIFFICULTY {EASY = 0, MEDIUM = 1, HARD = 2};

class Sudoku {

public:
    Sudoku(); // ctor
    ~Sudoku(); // dtor

    bool solveSudoku(const int currentRow = 0, const int currentCol = 0);
    bool isSolutionCorrect();
    void generateSudokuPuzzle();

    void modifyBoard(const int row, const int col, const int value);
    int getValue(const int row, const int col);

    DIFFICULTY difficulty = MEDIUM;

private:
    std::array<std::array<int, 9>, 9> board = {0};
    void setBoardEmpty();
    bool isRowValid(const int row, int const value);
    bool isColumnValid(const int col, const int value);
    bool isSubtableValid(const int row, const int col, const int value);
    bool generateRandomSolution();
    bool isMoveValid(const int row, const int col, const int value);
};

#endif // SUDOKU_H
