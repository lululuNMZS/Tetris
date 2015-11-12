#ifndef QBOARDDATA_H
#define QBOARDDATA_H

#include <QFrame>
#include <QObject>
#include <QVector>
#include <QTimer>
#include <QtDebug>
#include "qpiece.h"

namespace Ui {
class QBoardData;
}

class QBoardData : public QFrame
{
    Q_OBJECT

public:
    explicit QBoardData(QWidget *parent = 0);
    ~QBoardData();

    int timeoutTime() { return 1000 / (level + 1); }

    QPiece *getPiece() { return &curPiece; }        //传输显示模块所需要的数据 （方块的移动）
    int getCurX() { return curX; }
    int getCurY() { return curY; }

    bool isGameStart() { return isStarted ;}

    void initBoard();
    const QVector<QVector<PieceShape> >  *getBoardState();
    void setBoardState(int x, int y, PieceShape temp);

    void newPiece();
    void oneLineDown();
    void setDropDownData();     //不能下降时，设置数据
    void removeFullLines();

    void rotateForm();
    void moveLeft();
    void moveRight();
    void fastDropDown();

    int calculateScore(int num);
    QPiece *getNextPiece();
    int getScore();
    int getLevel();

    bool tryMove(QPiece &piece, int x, int y);

    void suspendGame();
    void goOnGame();

private:
    Ui::QBoardData *ui;

    enum {BoardWidth = 12, BoardHeight = 21};

    QVector<QVector<PieceShape> > boardState;
    QBasicTimer timer;
    QBasicTimer levelTimer;
    QPiece curPiece;
    QPiece nextPiece;
    int score;
    int level;
    int timeNum;
    int curX;
    int curY;
    bool isStarted;
    bool isPaused;


protected:
    void timerEvent(QTimerEvent *event);

signals:
    void showNextPieceSignal();
    void updateShowBoardSignal();
    void scoreChangeSignal();
    void levelChangeSignal();
};

#endif // QBOARDDATA_H
