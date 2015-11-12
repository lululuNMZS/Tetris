#include "qboarddata.h"
#include "ui_qboarddata.h"

QBoardData::QBoardData(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::QBoardData)
{
    ui->setupUi(this);
}

QBoardData::~QBoardData()
{
    delete ui;
}

const QVector<QVector<PieceShape> > *QBoardData::getBoardState()
{
    return &boardState;
}

void QBoardData::setBoardState(int x, int y, PieceShape temp)
{
    boardState[x][y] = temp;
}

void QBoardData::initBoard()
{
    nextPiece.setRandomShape();

    level = 1;
    score = 0;
    emit levelChangeSignal();
    emit scoreChangeSignal();

    timeNum = 0;
    levelTimer.start(1000,this);

    boardState.clear();
    QVector<PieceShape> temp(BoardHeight, NoShape);
    for ( int  i = 0 ; i < BoardWidth ; i++ )
    {
        boardState.push_back(temp);
    }
}

void QBoardData::newPiece()
{
    isStarted = true;

    curPiece = nextPiece;
    nextPiece.setRandomShape();         //安排下一个形状
    emit showNextPieceSignal();

    timer.start(timeoutTime(), this);

    /*---------设置新方块显示， 定位方块的中心点-----------*/
    curX = BoardWidth / 2;
    if ( curPiece.getPieceShape() == LineShape || curPiece.getPieceShape() == SquareShape )
    {
        curY = curPiece.maxY() - 1;     //设置初始坐标时，长横与方砖会与顶部空一行，因此需要减去一行
    }
    else
    {
       curY = curPiece.maxY();
    }

    if ( !tryMove(curPiece, curX, curY) )       //如果不能再造新方块(该初始位置不能显示方块)，game over
    {
        qDebug() << "Game Over";
        curPiece.setShape(NoShape);         /*----------防止game over后， 顶部再次刷新方块，导致显示区的重叠-----*/

        timer.stop();
        levelTimer.stop();
        isStarted = false;
    }
}

bool QBoardData::tryMove(QPiece &piece, int x, int y)           //用来判断该点是否可以移动过去
{
    for ( int i = 0 ; i < 4 ; i++ )
    {
        int moveX = x + piece.getX(i);
        int moveY = y + piece.getY(i);
        if ( moveX < 0 || moveX >= BoardWidth || moveY < 0 || moveY >= BoardHeight )
        {
            return false;
        }
        if ( NoShape != boardState[moveX][moveY] )
        {
            return false;
        }
    }

    curPiece = piece;
    curX = x;
    curY = y;

    emit updateShowBoardSignal();
    return true;
}

void QBoardData::oneLineDown()
{
    if ( !tryMove(curPiece, curX, curY + 1) )
    {
        setDropDownData();
    }
}

void QBoardData::setDropDownData()
{
    for ( int i = 0 ; i < 4 ; i++ )
    {
        int x = curX + curPiece.getX(i);
        int y = curY + curPiece.getY(i);
        boardState[x][y] = curPiece.getPieceShape();
    }

    this->removeFullLines();
    this->newPiece();                     //固定好方块后，开新方块
}

void QBoardData::rotateForm()
{
    QPiece temp = this->getPiece()->rotateLeft();
    tryMove( temp, curX, curY );
}

void QBoardData::moveLeft()
{
    tryMove( curPiece, curX - 1, curY );
}

void QBoardData::moveRight()
{
    tryMove( curPiece, curX + 1, curY );
}

void QBoardData::fastDropDown()
{

    while ( tryMove( curPiece, curX, curY + 1 ) )
    {
       ;
    }
    setDropDownData();
}

void QBoardData::removeFullLines()
{
    int scoreNum = 0;

    for ( int x = 0 ; x < BoardHeight ; x++ )
    {
        int fullNum = 0;
        for ( int y = 0 ; y < BoardWidth ; y++ )
        {
            if ( NoShape == boardState[y][x] )      //若一行中有空格，跳出
            {
                break;
            }
            fullNum++;          //否则，将格子数增加
        }

        /*  判断格子的数量  */
        if ( fullNum == BoardWidth )              //若满行
        {
            for ( int y = 0 ; y < BoardWidth ; y++ )    //消除这一行
            {
                boardState[y][x] = NoShape;
            }

            for ( int i = x ; i > 0 ; i-- )         //将被消除行上方的方块下移
            {
                for ( int j = 0 ; j < BoardWidth ; j++ )
                {
                    boardState[j][i] = boardState[j][i-1];
                }
            }

            scoreNum++;
        }
    }

    if ( scoreNum > 0 )
    {
        score += calculateScore(scoreNum);
        emit scoreChangeSignal();
    }
}

QPiece* QBoardData::getNextPiece()
{
    return &nextPiece;       //得到下一个方块的形状
}

void QBoardData::timerEvent(QTimerEvent *event)
{
    if ( timer.timerId() == event->timerId()  )
    {
        this->oneLineDown();
    }

    if ( levelTimer.timerId() == event->timerId() )
    {
        timeNum++;

        if ( timeNum % 100 == 0 )            //控制递增速度
        {
            level++;
            emit levelChangeSignal();
        }
        qDebug() << timeNum;
    }
}

int QBoardData::calculateScore(int num)
{
    switch(num)
    {
    case 0:
        return 0;
        break;
    case 1:
        return 1;
        break;
    case 2:
        return 3;
        break;
    case 3:
        return 5;
        break;
    case 4:
        return 7;
        break;

    defult:
        break;
    }
}

int QBoardData::getScore()
{
    return score;
}

int QBoardData::getLevel()
{
    return level;
}

void QBoardData::suspendGame()
{
    timer.stop();
    levelTimer.stop();
}

void QBoardData::goOnGame()
{
    timer.start(timeoutTime(), this);
    levelTimer.start(1000, this);
}
