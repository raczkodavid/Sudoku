#ifndef SOLVER_H
#define SOLVER_H

class Solver {

public:
    Solver(); // ctor
    ~Solver(); // dtor
    bool solveSudoku(int board[9][9], const int currentRow = 0, const int currentCol = 0);

private:
    bool isRowValid(int board[9][9], const int row, int const value);
    bool isColumnValid(int board[9][9], const int col, const int value);
    bool isSubtableValid(int board[9][9], const int row, const int col, const int value);
    bool isMoveValid(int board[9][9], const int row, const int col, const int value);
    bool isSolutionCorrect(int board[9][9]);
};

#endif // SOLVER_H
