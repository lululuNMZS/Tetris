#ifndef QSHOWBOARD_H
#define QSHOWBOARD_H

#include <QWidget>
#include "qpiece.h"

namespace Ui {
class QShowBoard;
}

class QShowBoard : public QWidget
{
    Q_OBJECT

public:
    explicit QShowBoard(QWidget *parent = 0);
    ~QShowBoard();

private:
    Ui::QShowBoard *ui;

private:
    enum {BoardWidth = 12, BoardHight = 21};
    int squarePixel;
    PieceShape nextPieceShape;

    const QVector<QVector<PieceShape> > *boardStateShow;
    int curX;
    int curY;
    QPiece *curPiece;
    QPiece *nextPiece;

protected:
    void paintEvent(QPaintEvent *);

public:
    void setBoardState(QVector<QVector<PieceShape> > const *);
    void drawSquare(QPainter &painter, int x, int y, PieceShape shape, int squarePixel);
    void setShowValue(QPiece *piece, int X, int Y);                 //设置方块轨迹显示所需要的数据
    QPixmap drawNextPieceLablePixmap(QPiece *piece);
};

#endif // QSHOWBOARD_H
