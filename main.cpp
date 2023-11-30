#include <QCoreApplication>
#include "battleship.h"

int main() {
    const QString filePath = "/home/topik/QtProjects/BattleshipConsole/battle_ship.txt";
    Battleship game(filePath);
    game.play();
    return 0;
}
