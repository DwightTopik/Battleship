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



    // TODO: Calculate counts ships errors
    errors = QVector<QString>(ships.size(), QString(""));

    return true;
}

void Battleship::printResult(bool isCorrect, const QString& explanation) const {
    if (!isCorrect) {
        std::cout << "Incorrect placement: " << explanation.toStdString() << "\n";
    }
}
