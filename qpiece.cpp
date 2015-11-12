#include "qpiece.h"
#include <QtWidgets>

QPiece::QPiece()
{

}

QPiece::~QPiece()
{

}

void QPiece::setRandomShape()
{
    int temp = qrand()%7 + 1;
    PieceShape shape = PieceShape(temp);

    this->setShape(shape);
}

void QPiece::setShape(PieceShape shape)
{
    //形状坐标表
    static const int coordinateTable[8][4][2] = {                   //反笛卡尔坐标系
        { { 0, 0 },    { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { -1, 1 },   { 0, 1 },   { -1, 0 },  { 0, 0 } },     //1 SquareShape   与顶部差一行 方块的maxY为基点，加上当前点的Y坐标来映射
        { { 0, -1 },   { 0, 0 },   { 0, 1 },   { 0, 2 } },     //2 LineShape    与顶部差一行
        { { 0, -1 },   { 0, 0 },   { 1, 0 },   { 1, 1 } },     //5 SShape
        { { 0, -1 },   { 0, 0 },   { -1, 0 },  { -1, 1 } },    //6 ZShape
        { { 0, -1 },   { 0, 0 },   { 0, 1 },   { 1, 1 } },     //3 LShape
        { { 0, -1 },   { 0, 0 },   { 0, 1 },   { -1, 1 } },    //4 OppsiteLShape
        { { 0, -1 },   { 0, 0 },   { 0, 1 },   { 1, 0 } }      //7 TShape
    };

    for ( int i = 0 ; i < 4 ; i++ )
    {
        for ( int j = 0 ; j < 2 ; j++ )
        {
            coords[i][j] = coordinateTable[shape][i][j];
        }
    }

    pieceShape = shape;
}

PieceShape QPiece::getPieceShape()
{
    return pieceShape;
}

QPiece QPiece::rotateLeft()
{
    if ( SquareShape == pieceShape )        //如果是方块
    {
        return *this;           //返回该形状， *this ：对当前项目取值
    }

    QPiece result;
    result.pieceShape = pieceShape;
    for ( int i = 0 ; i < 4 ; i++ )     //交换x，y的位置x = y， y = -x
    {
        result.coords[i][0] = coords[i][1];
        result.coords[i][1] = -coords[i][0];
    }

    return result;
}

int QPiece::minX() const
{
    int min = coords[0][0];
    for ( int i = 1 ; i < 4 ; i++ )
    {
        min = qMin( min, coords[i][0] );
    }
    return min;
}

int QPiece::maxX() const
{
    int max = coords[0][0];
    for ( int i = 1 ; i < 4 ; i++ )
    {
        max = qMax( max, coords[i][0] );
    }
    return max;
}

int QPiece::minY() const
{
    int min = coords[0][1];
    for ( int i = 1 ; i < 4 ; i++ )
    {
        min = qMin( min, coords[i][1] );
    }
    return min;
}

int QPiece::maxY() const
{
    int max = coords[0][1];
    for ( int i = 1 ; i < 4 ; i++ )
    {
        max = qMax( max, coords[i][1] );
    }
    return max;
}
