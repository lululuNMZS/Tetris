#include "qshowboard.h"
#include "ui_qshowboard.h"
#include <QtWidgets>

QShowBoard::QShowBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QShowBoard)
{
    ui->setupUi(this);

    squarePixel = 25;
    this->setFixedSize(BoardWidth*squarePixel, BoardHight*squarePixel);
}

QShowBoard::~QShowBoard()
{
    delete ui;
}

void QShowBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black,2));

    painter.drawRect(0,0, BoardWidth*squarePixel, BoardHight*squarePixel);

    for ( int i = 0 ; i < BoardWidth ; i++ )        //方块固定位置时显示
    {
        for ( int j = 0 ; j < BoardHight ; j++ )
        {
            if ( NoShape != (*boardStateShow)[i][j] )
            {
                this->drawSquare(painter, i, j, (*boardStateShow)[i][j], squarePixel) ;
            }
        }
    }

    for ( int i = 0 ; i < 4 ; i++ )                 //显示移动轨迹
    {
        int x = curX + curPiece->getX(i);
        int y = curY + curPiece->getY(i);
        this->drawSquare(painter, x, y, curPiece->getPieceShape(), squarePixel);
    }
}

QPixmap QShowBoard::drawNextPieceLablePixmap(QPiece *piece)      //将下一个形状画在pixmap上，并返回给nextLabel调用
{
    QPixmap pixmap(100, 60);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), this->palette().background());
    nextPiece = piece;

    PieceShape draw[5][5];
    for ( int x = 0 ; x < 5 ; x++ )
    {
        for ( int y = 0 ; y < 5 ; y++ )
        {
            draw[x][y] = NoShape;
        }
    }

    if ( nextPiece->getPieceShape() == LineShape )   //当nextPiece是长横时，将其横向显示，以适应扁长的nextLabel
    {
        QPiece temp = nextPiece->rotateLeft();
        for ( int i = 0 ; i < 4 ; i++ )
        {

            int x = temp.getX(i);
            int y = temp.getY(i);
            draw[x+2][y+1] = temp.getPieceShape();
        }
    }
    else
    {
        for ( int i = 0 ; i < 4 ; i++ )
        {

            int x = nextPiece->getX(i);
            int y = nextPiece->getY(i);
            draw[x+2][y+1] = nextPiece->getPieceShape();
        }
    }

    for ( int x = 0 ; x < 5 ; x++ )
    {
        for ( int y = 0 ; y < 5 ; y++ )
        {
            if ( NoShape != draw[x][y] )
            {
                drawSquare( painter, x, y, draw[x][y], 20);
            }
        }
    }

    return pixmap;
}

void QShowBoard::setBoardState(const QVector<QVector<PieceShape> > * temp)
{
    boardStateShow = temp;
}

void QShowBoard::drawSquare(QPainter &painter, int x, int y, PieceShape shape, int squarePixel)
{
    QBrush pieceColour;
    switch ( shape )
    {
    case LineShape:
            pieceColour = Qt::red;
            break;
    case SquareShape:
            pieceColour = Qt::blue;
            break;
    case  ZShape:
            pieceColour = Qt::darkRed;
            break;
    case   SShape:
            pieceColour = Qt::black;
            break;
    case  LShape:
            pieceColour = Qt::gray;
            break;
    case  OppsiteLShape:
            pieceColour = Qt::darkBlue;
            break;
    case   TShape:
            pieceColour = Qt::magenta;
            break;
    defult:
            break;
    }

    painter.fillRect(x*squarePixel, y*squarePixel, squarePixel, squarePixel, pieceColour);

    painter.setPen(QPen(Qt::white));
    painter.drawLine(x*squarePixel, y*squarePixel, x*squarePixel+squarePixel, y*squarePixel);
    painter.drawLine(x*squarePixel, y*squarePixel, x*squarePixel, y*squarePixel+squarePixel);
}

void QShowBoard::setShowValue(QPiece *piece, int X, int Y)
{
    curX = X;
    curY = Y;
    curPiece = piece;
}
