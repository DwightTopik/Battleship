#include <QCoreApplication>

int main() {
    const QString filePath = "/home/topik/QtProjects/BattleshipConsole/battle_ship.txt";
    battleship game(filePath);
    game.play();
    return 0;
}
