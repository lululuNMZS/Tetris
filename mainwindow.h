#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qboarddata.h"
#include "qshowboard.h"

#include <QLabel>
#include <QLCDNumber>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private:
    QWidget *window;
    QBoardData *boardData;
    QShowBoard *showBoard;
    QLabel *labelNext;

    QLCDNumber *LCDScore;
    QLCDNumber *LCDLevel;

    QTimer timer;

    int isPause;
    QLabel *createLabel(const QString text);    //创建label的快捷函数

public:
    void drawDesktop();
    void runGame();

public slots:
    void initGameSlot();
    void suspendGameSlot();

    void showNextPieceLabelSlot();
    void updateShowBoardSlot();
    void scoreChangeSlot();
    void levelChangeSlot();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
