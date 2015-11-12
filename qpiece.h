#ifndef QPIECE_H
#define QPIECE_H

#include <QObject>

enum PieceShape {NoShape, SquareShape, LineShape, ZShape, SShape, LShape, OppsiteLShape, TShape};

class QPiece
{
public:
    QPiece();
    ~QPiece();

    void setRandomShape();
    void setShape(PieceShape shape);
    PieceShape getPieceShape();
    QPiece rotateLeft();

    int getX(int i) { return coords[i][0]; }
    int getY(int i) { return coords[i][1]; }
    int minX() const;
    int maxX() const;
    int minY() const;
    int maxY() const;

private:
    int coords[4][2];
    PieceShape pieceShape;


};

#endif // QPIECE_H
