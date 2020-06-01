#include "snake.hpp"

int main() {
    system("mode con cols=100 lines=31");
    Pos(COLS / 4, ROWS / 3);
    cout << "请选择模式 1:经典 2:寻找真食物" << endl;
    Pos(COLS / 4, ROWS / 3 + 1);
    cout << "*只有退出游戏才能重新选择模式*";
    int mode;
    cin >> mode;
    system("cls");
    if (mode == 1) {
        gameMo1 Game;
        Game.InitGame();
        Game.GameEntry();
    } else if (mode == 2) {
        gameMo2 Game;
        Game.InitGame();
        Game.GameEntry();
    } else {
        gameMo1 Game;
        Game.InitGame();
        Game.GameEntry();
    }
    return 0;
}