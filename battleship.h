#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include <QString>
#include <QStringList>
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
    QVector<QVector<Cell>> shipsMap;
    QVector<QVector<QPoint>> ships;
    QStringList errors;
    bool readShipPlacement();
    void checkSurroundingShips(const QVector<QVector<QPoint>> &ships);
    void checkSurroundingShips(const QVector<QVector<QPoint>> &ships, int shipIndex, int testIndex);
    bool checkSurroundingDeck(QVector<QVector<Cell>> &shipMap, QPoint point, int deltaX, int deltaY);
    void detectShips(QVector<QVector<Cell>>& shipsGrid);
    int countShips(const QVector<QVector<QPoint>> &ships, int deck) const;
    void outputErrors() const;
    void outputErrors(const QStringList &errors) const;
};


#endif // BATTLESHIP_H
