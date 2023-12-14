#include "battleship.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <iostream>

Battleship::Battleship(const QString& filePath) : filePath(filePath) {}

void Battleship::play() {
    errors.clear();
    ships.clear();
    shipsMap.clear();

    if (!readShipPlacement()) {
        outputErrors(errors);
        return;
    }
    std::cout << "The ship map is correct." << std::endl;

    detectShips(shipsMap);

    std::sort(ships.begin(), ships.end(), [](const QVector<QPoint> &first, const QVector<QPoint> &second) {
        return first.size() > second.size();
    });

    std::cout << "Ship map:" << std::endl;
    for (int index = 0; index < ships.size(); ++index) {
        QString points = "";
        for (const auto &deck : ships.at(index)) {
            if (!points.isEmpty()) {
                points += ", ";
            }
            points += QString("(%1, %2)").arg(deck.x()).arg(deck.y());
        }

        QString info = QString("%1. %2\'deck ship at coordinates {%3}").arg(index).arg(ships.at(index).size()).arg(points);
        std::cout << info.toLocal8Bit().data() << std::endl;
    }

    for (int i = 4; i != 0; --i) {
        int localCountShips = countShips(ships, i);
        int needShipsCount = 5 - i;
        if (localCountShips != needShipsCount) {
            errors.append(QString("%1\'deck ships are %2. %3 ships: %4.").arg(i).arg(localCountShips)
                              .arg(localCountShips < needShipsCount ? "Missing" : "Exceeding")
                              .arg(abs(localCountShips - needShipsCount)));
        }
    }

    checkSurroundingShips(ships);

    if (errors.isEmpty()) {
        std::cout << "Excellent, you're a great commander!" << std::endl;
        return;
    }

    outputErrors(errors);
}

bool Battleship::readShipPlacement() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errors.append("The ship map is invalid.");
        return false;
    }

    QTextStream in(&file);
    int rowCount = 0;
    QVector<QVector<Cell>> localShipsGrid(10, QVector<Cell>(10, Cell()));
    ships.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        ++rowCount;
        if (line.length() != 10) {
            errors.append("Invalid row length.");
            return false;
        }
        for (int col = 0; col < line.length(); ++col) {
            QChar cell = line.at(col);
            if (cell != '*' && cell != '0') {
                errors.append("Invalid character in the map.");
                return false;
            }
            if (cell == '*') {
                localShipsGrid[rowCount - 1][col].ship = true;
            }
        }
    }

    if (rowCount != 10) {
        errors.append("Invalid number of rows.");
        return false;
    }
    shipsMap = localShipsGrid;

    return true;
}

void Battleship::checkSurroundingShips(const QVector<QVector<QPoint>> &ships)
{
    for (int i = 0; i < ships.size(); ++i) {
        for (int j = i + 1; j < ships.size(); ++j) {
            checkSurroundingShips(ships, i, j);
        }
    }
}

void Battleship::checkSurroundingShips(const QVector<QVector<QPoint>> &ships, int shipIndex, int testIndex)
{
    for (const auto &decksShip : ships.at(shipIndex)) {
        for (const auto &decksTest : ships.at(testIndex)) {
            if ((abs(decksShip.x() - decksTest.x()) < 2) && (abs(decksShip.y() - decksTest.y()) < 2)) {
                errors.append(QString("Ships with indexes: %1 and %2 are in contact. Points (%3, %4) and (%5, %6)")
                                  .arg(shipIndex).arg(testIndex)
                                  .arg(decksShip.x())
                                  .arg(decksShip.y())
                                  .arg(decksTest.x())
                                  .arg(decksTest.y()));
            }
        }
    }
}

void Battleship::detectShips(QVector<QVector<Cell>>& shipsGrid) {
    for (int row = 0; row < shipsGrid.size(); ++row) {
        for (int col = 0; col < shipsGrid.at(row).size(); ++col) {
            if (shipsGrid[row][col].check || !shipsGrid[row][col].ship) {
                continue;
            }
            shipsGrid[row][col].check = true;
            ships.push_back({QPoint(col, row)});
            QVector<QPoint>& ship = ships.back();
            const QPoint& start = ship.front();
            int delta = 1;
            while (checkSurroundingDeck(shipsGrid, start, delta, 0)) {
                ship.push_back(QPoint(start.x() + delta, start.y()));
                if (++delta > 3) {
                    break;
                }
            }
            if (ship.size() > 1) {
                continue;
            }
            delta = 1;
            while (checkSurroundingDeck(shipsGrid, start, 0, delta)) {
                ship.push_back(QPoint(start.x(), start.y() + delta));
                if (++delta > 3) {
                    break;
                }
            }
        }
    }
}

int Battleship::countShips(const QVector<QVector<QPoint> > &ships, int deck) const
{
    return std::count_if(ships.begin(), ships.end(), [deck](const QVector<QPoint>& ship) { return ship.size() == deck; });
}

bool Battleship::checkSurroundingDeck(QVector<QVector<Cell> > &shipMap, QPoint point, int deltaX, int deltaY)
{
    if ((point.x() + deltaX >= shipMap.at(point.y()).size()) || (point.y() + deltaY >= shipMap.size())) {
        return false;
    }
    shipMap[point.y() + deltaY][point.x() + deltaX].check = true;
    return shipMap.at(point.y() + deltaY).at(point.x() + deltaX).ship;
}

void Battleship::outputErrors(const QStringList &errors) const {
    std::cout << "Errors:\n";
    int i = 1;
    for (const QString &error : errors) {
        if (error.isEmpty()) {
            continue;
        }
        std::cout << "Error " << i++ << ": " << error.toLocal8Bit().data() << "\n";
    }
}
