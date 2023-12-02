#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include <QString>
#include <QVector>
#include <QPoint>

struct Cell
{
    bool ship {false};
    bool check {false};
};

class Battleship {
public:
    Battleship(const QString& filePath);
    void play();

private:
    QString filePath;
    QVector<QVector<QPoint>> ships;
    QVector<QString> errors;
    bool checkShipPlacement();
    void printResult(bool isCorrect, const QString& explanation) const;
    bool checkSurroundingCell(QVector<QVector<Cell>> &shipGrid, QPoint point, int deltaX, int deltaY);
    void countShips(QVector<QVector<Cell>>& shipGrid);
    void outputShipCounts() const;
};


#endif // BATTLESHIP_H
