#include "snake.hpp"

gameBase* GamePTR = NULL;

void Pos(int x, int y) {
    COORD pos;
    HANDLE Output;
    pos.X = x;
    pos.Y = y;
    Output = GetStdHandle(
        STD_OUTPUT_HANDLE);  //���ر�׼�����롢����������豸�ľ����Ҳ���ǻ�����롢���/�������Ļ�������ľ��
    SetConsoleCursorPosition(Output, pos);
}

void gameBase::InitMap() {
    system("color F3");
    int i;
    for (i = 0; i <= COLS; i += 2)  //��ӡ���±߿�
    {
        Pos(i, 0);
        cout << "��";
        Pos(i, ROWS - 1);
        cout << "��";
    }
    for (i = 0; i <= ROWS; i++)  //��ӡ���ұ߿�
    {
        Pos(1, i);
        cout << "��";
        Pos(COLS - 2, i);
        cout << "��";
    }
}

int gameBase::GetAdd(int sleeptime) { return (350 - sleeptime) / 15; }

void gameBase::Pause() {
    while (1) {
        Sleep(300);
        if (GetAsyncKeyState(VK_SPACE)) {
            break;
        }
    }
}

void gameBase::DeleteGame() {
    delete[] & Snake;
    delete[] head;
    delete &Food;
    delete &Para;
}

void snake::InitSnake() {
    snake* tail;
    tail = new snake(9, 9);
    for (int i = 1; i <= 4; i++)  //��ʼ����Ϊ5
    {
        GamePTR->head = new snake(9 + 2 * i, 9);
        GamePTR->head->next = tail;
        tail = GamePTR->head;
    }
    while (tail != NULL)  //��ͷ��β���������
    {
        Pos(tail->x, tail->y);
        cout << '+';
        tail = tail->next;
    }
}

void snake::BiteSelf() {
    snake* self;
    self = GamePTR->head->next;
    while (self != NULL) {
        if (self->x == GamePTR->head->x && self->y == GamePTR->head->y) {
            GamePTR->EndGame(2);
        }
        self = self->next;
    }
}

void snake::HitWall() {
    if (GamePTR->head->x <= 1 || GamePTR->head->x >= COLS - 2 ||
        GamePTR->head->y <= 0 || GamePTR->head->y >= ROWS - 1) {
        GamePTR->EndGame(1);
    }
}

snake* snake::SnakeMove() {
    HitWall();
    snake* nexthead = new snake();
    switch (GamePTR->Para.dir) {
        case U:
            nexthead->x = GamePTR->head->x;
            nexthead->y = GamePTR->head->y - 1;
            break;
        case D:
            nexthead->x = GamePTR->head->x;
            nexthead->y = GamePTR->head->y + 1;
            break;
        case L:
            nexthead->x = GamePTR->head->x - 2;
            nexthead->y = GamePTR->head->y;
            break;
        case R:
            nexthead->x = GamePTR->head->x + 2;
            nexthead->y = GamePTR->head->y;
            break;
    }
    BiteSelf();
    nexthead->next = GamePTR->head;
    GamePTR->head = nexthead;  //�á����ڵ���ͷ��ָ���ɡ�δ������ͷ��ָ��
    return nexthead;
}

void snake::SpeedUp() {
    if (GamePTR->Para.sleeptime >= 50) {
        GamePTR->Para.sleeptime -= 30;
        GamePTR->Para.add = GamePTR->GetAdd(GamePTR->Para.sleeptime);
    }
}

void snake::SpeedDown() {
    if (GamePTR->Para.sleeptime < 320) {
        GamePTR->Para.sleeptime += 30;
        GamePTR->Para.add = GamePTR->GetAdd(GamePTR->Para.sleeptime);
    }
}

void snake::PrintSnake(snake* nexthead) {
    // if (nexthead->x == GamePTR->Food.x &&
    //     nexthead->y == GamePTR->Food.y)  //�����һ����ʳ��
    // {
    if (EatFood(GamePTR->Food, *nexthead)) {
        Pos(GamePTR->head->x, GamePTR->head->y);  //ֻҪ��ʳ�ﻻ�ɡ�δ������ͷ��
        cout << '+';
        GamePTR->Para.score += GamePTR->Para.add;
        GamePTR->Para.FoodEaten = true;
    } else  //���û��ʳ�� Ҫ���ߵ�ÿһ����ǰ��
    {
        snake* temp = GamePTR->head;  //���幤��ָ��
        while (temp->next->next != NULL)  //�������ڵ��ߡ��ĵڶ�/��/��...�ڲ�Ϊ��
        {
            Pos(temp->x, temp->y);
            cout << '+';  //�����δ�����ߡ�����ͷ/�ڶ�/��...��
            temp = temp->next;  //����������
        }
        Pos(temp->next->x,
            temp->next->y);  //���������ѭ�� ����ָ��ָ�����ڵ��ߡ��ĵ����ڶ���
                             //����δ�����ߡ�����β
        cout << ' ';  //ȥ�������ڵ��ߡ������һ��
        temp->next = NULL;
        delete[] temp->next;
    }
}

bool EatFood(food Food, snake Snake) {
    return Food.x == Snake.x && Food.y == Snake.y ? true : false;
}

void food::CreateFood() {
    food* foodtemp = new food();
    do {
        foodtemp->x = (rand() % (COLS - 4)) + 2;
    } while (foodtemp->x % 2 == 0);
    foodtemp->y = rand() % (ROWS - 2) + 1;
    snake* temp = GamePTR->head;
    while (temp->next) {
        if (EatFood(*foodtemp, *temp)) {
            delete[] foodtemp;
            return;
        }
        temp = temp->next;
    }
    x = foodtemp->x;
    y = foodtemp->y;
    Pos(x, y);
    cout << '$';
    GamePTR->Para.FoodEaten = false;
    delete[] foodtemp;
    return;
}

void gameMo1::RunGame() {
    Para.dir = R;
    while (1) {
        if (GetAsyncKeyState(VK_UP) && Para.dir != D) {
            Para.dir = U;
        } else if (GetAsyncKeyState(VK_DOWN) && Para.dir != U) {
            Para.dir = D;
        } else if (GetAsyncKeyState(VK_LEFT) && Para.dir != R) {
            Para.dir = L;
        } else if (GetAsyncKeyState(VK_RIGHT) && Para.dir != L) {
            Para.dir = R;
        } else if (GetAsyncKeyState(VK_SPACE)) {
            Pause();
        } else if (GetAsyncKeyState(VK_ESCAPE)) {
            EndGame(3);
        } else if (GetAsyncKeyState(VK_F1)) {
            Snake.SpeedUp();
        } else if (GetAsyncKeyState(VK_F2)) {
            Snake.SpeedDown();
        }
        Sleep(Para.sleeptime);
        snake* nexthead = Snake.SnakeMove();
        Snake.PrintSnake(nexthead);
        while (Para.FoodEaten) {
            Food.CreateFood();
        }
        Pos(COLS / 5, ROWS);
        cout << "�÷֣�" << Para.score;
        Pos(COLS / 5 + 10, ROWS);
        cout << "ÿ��ʳ��÷֣�" << Para.add << ' ';
    }
}

void gameMo1::InitGame() {
    GamePTR = this;
    Pos(COLS / 4, ROWS / 3);
    cout << "�á�.��.��.���ֱ�����ߵ��ƶ��� F1 Ϊ���٣�F2 Ϊ����" << endl;
    Pos(COLS / 4, ROWS / 3 + 1);
    cout << "���ٽ��ܵõ����ߵķ�����" << endl;
    Pos(COLS / 4, ROWS / 2);
    cout << endl << "��ѡ���Ѷ� 0:�� 1:��ͨ 2:���� ";
    cin >> Para.diff;
    switch (Para.diff) {
        case 0:
            Para.sleeptime = 290;
            Para.add = GetAdd(Para.sleeptime);
            break;
        case 1:
            Para.sleeptime = 200;
            Para.add = GetAdd(Para.sleeptime);
            break;
        case 2:
            Para.sleeptime = 50;
            Para.add = GetAdd(Para.sleeptime);
            break;
        default:
            Para.sleeptime = 200;
            Para.add = GetAdd(Para.sleeptime);
            break;
    }
}

void gameMo1::EndGame(int EndGameStatus) {
    system("cls");
    Pos(COLS / 3, ROWS / 3);
    if (EndGameStatus == 1) {
        cout << "�Բ�����ײ��ǽ�ˡ���Ϸ������";
    } else if (EndGameStatus == 2) {
        cout << "�Բ�����ҧ���Լ��ˡ���Ϸ������";
    } else if (EndGameStatus == 3) {
        cout << "��������������Ϸ��";
    } else {
        cout << "δ֪����";
    }
    Pos(COLS / 3, ROWS / 3 + 1);
    cout << "���ĵ÷��ǣ�" << Para.score << endl;
    cout << "Quit/Restart? [q/r]" << endl;
    char ch;
    while (1) {
        cin >> ch;
        if (ch == 'r') {
            system("cls");
            DeleteGame();
            GameEntry();
        } else if (ch == 'q') {
            exit(0);
        } else {
            cout << "Quit/Restart? [q/r]" << endl;
        }
    }
}

void gameMo1::GameEntry() {
    Para.FoodEaten = false;
    Para.score = 0;
    system("cls");
    srand((unsigned)time(NULL));  //Ϊ�˷�ֹÿ�β������������ͬ����������Ϊtime
    InitMap();
    Snake.InitSnake();
    Food.CreateFood();
    cin.sync();
    RunGame();
}

void gameMo2::RunGame() {
    Para.dir = R;
    while (1) {
        if (GetAsyncKeyState(VK_UP) && Para.dir != D) {
            Para.dir = U;
        } else if (GetAsyncKeyState(VK_DOWN) && Para.dir != U) {
            Para.dir = D;
        } else if (GetAsyncKeyState(VK_LEFT) && Para.dir != R) {
            Para.dir = L;
        } else if (GetAsyncKeyState(VK_RIGHT) && Para.dir != L) {
            Para.dir = R;
        } else if (GetAsyncKeyState(VK_SPACE)) {
            Pause();
        } else if (GetAsyncKeyState(VK_ESCAPE)) {
            EndGame(3);
        } else if (GetAsyncKeyState(VK_F1)) {
            Snake.SpeedUp();
        } else if (GetAsyncKeyState(VK_F2)) {
            Snake.SpeedDown();
        }
        Sleep(Para.sleeptime);
        snake* nexthead = Snake.SnakeMove();
        Snake.PrintSnake(nexthead);
        while (Para.FoodEaten) {
            Food.CreateFood();
            fake = false;
        }
        if (fake == false) {
            for (int i = 0; i < rand() % 20; i++) Food.CreateFood();
            fake = true;
        }
        Pos(COLS / 5, ROWS);
        cout << "�÷֣�" << Para.score;
        Pos(COLS / 5 + 10, ROWS);
        cout << "ÿ��ʳ��÷֣�" << Para.add << ' ';
    }
}

void gameMo2::InitGame() {
    GamePTR = this;
    Pos(COLS / 4, ROWS / 3 - 1);
    cout << "ÿ�ֻ���������20����ʳ�ȥѰ����ʳ��ɣ�" << endl;
    Pos(COLS / 4, ROWS / 3);
    cout << "�á�.��.��.���ֱ�����ߵ��ƶ��� F1 Ϊ���٣�F2 Ϊ����" << endl;
    Pos(COLS / 4, ROWS / 3 + 1);
    cout << "���ٽ��ܵõ����ߵķ�����" << endl;
    Pos(COLS / 4, ROWS / 2);
    cout << endl << "��ѡ���Ѷ� 0:�� 1:��ͨ 2:���� ";
    cin >> Para.diff;
    switch (Para.diff) {
        case 0:
            Para.sleeptime = 290;
            Para.add = 4;
            break;
        case 1:
            Para.sleeptime = 200;
            Para.add = 10;
            break;
        case 2:
            Para.sleeptime = 50;
            Para.add = 20;
            break;
        default:
            Para.sleeptime = 200;
            Para.add = 10;
            break;
    }
}

void gameMo2::EndGame(int EndGameStatus) {
    system("cls");
    Pos(COLS / 3, ROWS / 3);
    if (EndGameStatus == 1) {
        cout << "�Բ�����ײ��ǽ�ˡ���Ϸ������";
    } else if (EndGameStatus == 2) {
        cout << "�Բ�����ҧ���Լ��ˡ���Ϸ������";
    } else if (EndGameStatus == 3) {
        cout << "��������������Ϸ��";
    } else {
        cout << "δ֪����";
    }
    Pos(COLS / 3, ROWS / 3 + 1);
    cout << "���ĵ÷��ǣ�" << Para.score << endl;
    cout << "Quit/Restart? [q/r]" << endl;
    char ch;
    while (1) {
        cin >> ch;
        if (ch == 'r') {
            system("cls");
            DeleteGame();
            GameEntry();
        } else if (ch == 'q') {
            exit(0);
        } else {
            cout << "Quit/Restart? [q/r]" << endl;
        }
    }
}

void gameMo2::GameEntry() {
    Para.FoodEaten = false;
    Para.score = 0;
    system("cls");
    srand((unsigned)time(NULL));  //Ϊ�˷�ֹÿ�β������������ͬ����������Ϊtime
    InitMap();
    Snake.InitSnake();
    Food.CreateFood();
    cin.sync();
    RunGame();
}
