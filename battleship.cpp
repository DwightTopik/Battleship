#include "battleship.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <iostream>

Battleship::Battleship(const QString& filePath) : filePath(filePath) {}

void Battleship::play() {
    if (checkShipPlacement()) {
        std::cout << "Ships are correctly placed.\n";
    } else {
        std::cout << "Ships are incorrectly placed.\n";
    }
}

bool Battleship::checkShipPlacement() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        printResult(false, "Could not open the file.");
        return false;
    }

    QTextStream in(&file);
    int rowCount = 0;
    QVector<QVector<Cell>> shipGrid(10, QVector<Cell>(10, Cell()));
    ships.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        ++rowCount;
        if (line.length() != 10) {
            printResult(false, "Invalid row length.");
            return false;
        }
        for (int col = 0; col < 10; ++col) {
            QChar cell = line.at(col);
            if (cell != '*' && cell != '0') {
                printResult(false, "Invalid character in the file.");
                return false;
            }
            if (cell == '*') {
                if (shipGrid[rowCount - 1][col].ship) {
                    printResult(false, "Ships cannot touch.");
                    return false;
                }
                shipGrid[rowCount - 1][col].ship = true;
            }
        }
    }

    if (rowCount != 10) {
        printResult(false, "Invalid number of rows.");
        return false;
    }

    countShips(shipGrid);

    // TODO: Calculate counts ships errors
    errors = QVector<QString>(ships.size(), QString(""));

    return true;
}

void Battleship::printResult(bool isCorrect, const QString& explanation) const {
    if (!isCorrect) {
        std::cout << "Incorrect placement: " << explanation.toStdString() << "\n";
    }
}

void Battleship::countShips(QVector<QVector<Cell>>& shipGrid) {
    int singleDeckCount = 0;
    int doubleDeckCount = 0;
    int tripleDeckCount = 0;
    int quadrupleDeckCount = 0;

    for (int row = 0; row < shipGrid.size(); ++row) {
        for (int col = 0; col < shipGrid.at(row).size(); ++col) {
            if (shipGrid[row][col].check || !shipGrid[row][col].ship) {
                continue;
            }
            shipGrid[row][col].check = true;
            ships.push_back({QPoint(col, row)});
            QVector<QPoint>& ship = ships.back();
            const QPoint& start = ship.front();
            int delta = 1;
            while (checkSurroundingCell(shipGrid, start, delta, 0)) {
                ship.push_back(QPoint(start.x() + delta, start.y()));
                if (++delta > 3) {
                    break;
                }
            }
            if (ship.size() > 1) {
                continue;
            }
            delta = 1;
            while (checkSurroundingCell(shipGrid, start, 0, delta)) {
                ship.push_back(QPoint(start.x(), start.y() + delta));
                if (++delta > 3) {
                    break;
                }
            }

            // Count the type of ship
            int shipSize = ship.size();
            if (shipSize == 1) {
                ++singleDeckCount;
            } else if (shipSize == 2) {
                ++doubleDeckCount;
            } else if (shipSize == 3) {
                ++tripleDeckCount;
            } else if (shipSize == 4) {
                ++quadrupleDeckCount;
            }
        }
    }

    if (singleDeckCount != 4 || doubleDeckCount != 3 || tripleDeckCount != 2 || quadrupleDeckCount != 1) {
        printResult(true, "Incorrect number of ships.");
    }
}



void Battleship::outputShipCounts() const {
    std::cout << "Ship counts:\n";
    std::cout << "Single-deck ships: " << std::count_if(ships.begin(), ships.end(), [](const QVector<QPoint>& ship) { return ship.size() == 1; }) << "\n";
    std::cout << "Double-deck ships: " << std::count_if(ships.begin(), ships.end(), [](const QVector<QPoint>& ship) { return ship.size() == 2; }) << "\n";
    std::cout << "Triple-deck ships: " << std::count_if(ships.begin(), ships.end(), [](const QVector<QPoint>& ship) { return ship.size() == 3; }) << "\n";
    std::cout << "Quadruple-deck ships: " << std::count_if(ships.begin(), ships.end(), [](const QVector<QPoint>& ship) { return ship.size() == 4; }) << "\n";
}

bool Battleship::checkSurroundingCell(QVector<QVector<Cell> > &shipGrid, QPoint point, int deltaX, int deltaY)
{
    if ((point.x() + deltaX >= shipGrid.at(point.y()).size()) || (point.y() + deltaY >= shipGrid.size())) {
        return false;
    }
    shipGrid[point.y() + deltaY][point.x() + deltaX].check = true;
    return shipGrid.at(point.y() + deltaY).at(point.x() + deltaX).ship;
}
