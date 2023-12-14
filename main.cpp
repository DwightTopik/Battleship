#include <QCoreApplication>
#include <QDebug>
#include "battleship.h"

int main(int argc, char **argv) {
    QString fileName = "battle_ship.txt";
    QCoreApplication app(argc, argv);
    auto arguments = app.arguments();
    Battleship game(QCoreApplication::applicationDirPath() + "/" + fileName);
    game.play();
    return 0;
}
