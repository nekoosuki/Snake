#include "snake.hpp"

int main() {
    system("mode con cols=100 lines=31");
    Pos(COLS / 4, ROWS / 3);
    cout << "��ѡ��ģʽ 1:���� 2:Ѱ����ʳ��" << endl;
    Pos(COLS / 4, ROWS / 3 + 1);
    cout << "*ֻ���˳���Ϸ��������ѡ��ģʽ*";
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