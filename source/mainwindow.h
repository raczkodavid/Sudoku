#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include <QLineEdit>
#include "sudoku.h"
#include <QComboBox>

class QLabel;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void shuffleButtonClicked();
    void checkSolutionButtonClicked();
    void showSolutionButtonClicked();
    void updateTimer();

private:
    bool isSolutionCorrect();
    void revealSolution();
    void setupUI();
    void setupGrid();
    void updateBoard();
    void showBoardOnScreen();

    void startTimer();
    void stopTimer();
    void resetTimer();
    void disableInput();
    void resetCells();
    void difficultyChanged(int idx);

    QTimer* timer;
    QLabel* timerLabel;
    QPushButton* shuffleButton;
    QPushButton* checkSolutionButton;
    QPushButton* showSolutionButton;
    QComboBox* difficultyComboBox;
    QGridLayout* gridLayout; // Layout for the grid
    QLineEdit* cells[9][9]; // 2D array for the cells
    Sudoku* sudoku;
    int elapsedTime;
};

#endif // MAINWINDOW_H
