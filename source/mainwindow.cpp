#include "mainwindow.h"
#include "sudoku.h"
#include <QTimer>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    timer(new QTimer(this)),
    sudoku(new Sudoku()),
    elapsedTime(0) {

    setWindowIcon(QIcon(":/images/resources/sudokuico.ico"));
    this->setFixedSize(10, 10);
    setupUI();

    connect(shuffleButton,       &QPushButton::clicked, this, &MainWindow::shuffleButtonClicked);
    connect(checkSolutionButton, &QPushButton::clicked, this, &MainWindow::checkSolutionButtonClicked);
    connect(showSolutionButton,  &QPushButton::clicked, this, &MainWindow::showSolutionButtonClicked);
    connect(timer,               &QTimer::timeout,      this, &MainWindow::updateTimer);
    connect(difficultyComboBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::difficultyChanged);

    shuffleButtonClicked();
}

void MainWindow::setupUI() {
    QWidget* centralWidget  = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Top layout for timer and difficulty selection
    QHBoxLayout* topLayout  = new QHBoxLayout();

    // Timer display
    timerLabel = new QLabel("0 s", this);
    topLayout->addWidget(timerLabel);

    // Spacer to push the difficulty combo box to the right
    topLayout->addStretch(1);

    // Difficulty dropdown
    difficultyComboBox = new QComboBox(this);
    difficultyComboBox->addItem("EASY");
    difficultyComboBox->addItem("MEDIUM");
    difficultyComboBox->addItem("HARD");

    difficultyComboBox->setCurrentIndex(1); // Medium

    topLayout->addWidget(difficultyComboBox);

    mainLayout->addLayout(topLayout);

    // Grid layout
    gridLayout = new QGridLayout();
    setupGrid();
    mainLayout->addLayout(gridLayout);

    // Control buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    shuffleButton       = new QPushButton("Shuffle", this);
    checkSolutionButton = new QPushButton("Check Solution", this);
    showSolutionButton  = new QPushButton("Show Solution", this);

    buttonLayout->addWidget(shuffleButton);
    buttonLayout->addWidget(checkSolutionButton);
    buttonLayout->addWidget(showSolutionButton);

    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::setupGrid() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QLineEdit* cell = new QLineEdit(this);
            cell->setFixedSize(40, 40);
            cell->setAlignment(Qt::AlignCenter);
            cell->setMaxLength(1);
            cell->setText("0"); // initially every cell displays 0

            cells[row][col] = cell;
            gridLayout->addWidget(cell, row, col);
        }
    }
}

void MainWindow::difficultyChanged(int idx) {
    sudoku->difficulty = static_cast<DIFFICULTY>(idx);
    shuffleButtonClicked();
}

void MainWindow::disableInput() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (sudoku->getValue(i,j) != 0) {
                cells[i][j]->setReadOnly(true);
                cells[i][j]->setStyleSheet("QLineEdit {background-color: pink; border: none}");
            }
            else cells[i][j]->setStyleSheet("QLineEdit {background-color: lightblue; border: none}");
        }
    }
}

void MainWindow::resetCells() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (sudoku->getValue(i,j) != 0) {
                cells[i][j]->setReadOnly(false);
                cells[i][j]->setStyleSheet("");
                cells[i][j]->setText("0");
            }
        }
    }
}

void MainWindow::updateBoard() {
    for (int i = 0; i <9; ++i) {
        for (int j = 0; j < 9; ++j) {
            sudoku->modifyBoard(i, j, cells[i][j]->text().toInt());
        }
    }
}

void MainWindow::showBoardOnScreen() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cells[i][j]->setText(QString::number(sudoku->getValue(i, j)));
        }
    }
}

void MainWindow::shuffleButtonClicked() {
    resetCells();
    sudoku->generateSudokuPuzzle();
    disableInput();
    showBoardOnScreen();

    resetTimer();
    startTimer();
}

void MainWindow::checkSolutionButtonClicked() {
    if (isSolutionCorrect()) {
        QMessageBox::information(this, "Solution", "Correct!");
    } else {
        QMessageBox::warning(this, "Solution", "Incorrect. Try again!");
    }
}

void MainWindow::showSolutionButtonClicked() {
    revealSolution();
    stopTimer();
}

void MainWindow::updateTimer() {
    elapsedTime++;
    timerLabel->setText(QString::number(elapsedTime) + " s");
}

void MainWindow::startTimer() {
    timer->start(1000); // Update every second
}

void MainWindow::stopTimer() {
    timer->stop();
}

void MainWindow::resetTimer() {
    timer->stop();
    elapsedTime = 0;
    timerLabel->setText("0 s");
}

bool MainWindow::isSolutionCorrect() {
    // store the board
    updateBoard();
    return sudoku->isSolutionCorrect();
}

void MainWindow::revealSolution() {
    sudoku->solveSudoku();
    // update the cells
    showBoardOnScreen();
    stopTimer();
}
