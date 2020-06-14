#include "snake.hpp"

gameBase* GamePTR = NULL;

void Pos(int x, int y) {
    COORD pos;
    HANDLE Output;
    pos.X = x;
    pos.Y = y;
    Output = GetStdHandle(
        STD_OUTPUT_HANDLE);  //返回标准的输入、输出或错误的设备的句柄，也就是获得输入、输出/错误的屏幕缓冲区的句柄
    SetConsoleCursorPosition(Output, pos);
}

void gameBase::InitMap() {
    system("color F3");
    int i;
    for (i = 0; i <= COLS; i += 2)  //打印上下边框
    {
        Pos(i, 0);
        cout << "■";
        Pos(i, ROWS - 1);
        cout << "■";
    }
    for (i = 0; i <= ROWS; i++)  //打印左右边框
    {
        Pos(1, i);
        cout << "■";
        Pos(COLS - 2, i);
        cout << "■";
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
    for (int i = 1; i <= 4; i++)  //初始长度为5
    {
        GamePTR->head = new snake(9 + 2 * i, 9);
        GamePTR->head->next = tail;
        tail = GamePTR->head;
    }
    while (tail != NULL)  //从头到尾，输出蛇身
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
    GamePTR->head = nexthead;  //让“现在的蛇头”指针变成“未来的蛇头”指针
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
    //     nexthead->y == GamePTR->Food.y)  //如果下一个有食物
    // {
    if (EatFood(GamePTR->Food, *nexthead)) {
        Pos(GamePTR->head->x, GamePTR->head->y);  //只要将食物换成“未来的蛇头”
        cout << '+';
        GamePTR->Para.score += GamePTR->Para.add;
        GamePTR->Para.FoodEaten = true;
    } else  //如果没有食物 要把蛇的每一节往前移
    {
        snake* temp = GamePTR->head;  //定义工作指针
        while (temp->next->next != NULL)  //当“现在的蛇”的第二/三/四...节不为空
        {
            Pos(temp->x, temp->y);
            cout << '+';  //输出“未来的蛇”的蛇头/第二/三...节
            temp = temp->next;  //继续遍历蛇
        }
        Pos(temp->next->x,
            temp->next->y);  //经过上面的循环 工作指针指向“现在的蛇”的倒数第二节
                             //即“未来的蛇”的蛇尾
        cout << ' ';  //去除“现在的蛇”的最后一节
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
        cout << "得分：" << Para.score;
        Pos(COLS / 5 + 10, ROWS);
        cout << "每个食物得分：" << Para.add << ' ';
    }
}

void gameMo1::InitGame() {
    GamePTR = this;
    Pos(COLS / 4, ROWS / 3);
    cout << "用↑.↓.←.→分别控制蛇的移动， F1 为加速，F2 为减速" << endl;
    Pos(COLS / 4, ROWS / 3 + 1);
    cout << "加速将能得到更高的分数。" << endl;
    Pos(COLS / 4, ROWS / 2);
    cout << endl << "请选择难度 0:简单 1:普通 2:困难 ";
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
        cout << "对不起，您撞到墙了。游戏结束。";
    } else if (EndGameStatus == 2) {
        cout << "对不起，您咬到自己了。游戏结束。";
    } else if (EndGameStatus == 3) {
        cout << "您主动结束了游戏。";
    } else {
        cout << "未知错误";
    }
    Pos(COLS / 3, ROWS / 3 + 1);
    cout << "您的得分是：" << Para.score << endl;
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
    srand((unsigned)time(NULL));  //为了防止每次产生的随机数相同，种子设置为time
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
        cout << "得分：" << Para.score;
        Pos(COLS / 5 + 10, ROWS);
        cout << "每个食物得分：" << Para.add << ' ';
    }
}

void gameMo2::InitGame() {
    GamePTR = this;
    Pos(COLS / 4, ROWS / 3 - 1);
    cout << "每轮会生成至多20个假食物，去寻找真食物吧！" << endl;
    Pos(COLS / 4, ROWS / 3);
    cout << "用↑.↓.←.→分别控制蛇的移动， F1 为加速，F2 为减速" << endl;
    Pos(COLS / 4, ROWS / 3 + 1);
    cout << "加速将能得到更高的分数。" << endl;
    Pos(COLS / 4, ROWS / 2);
    cout << endl << "请选择难度 0:简单 1:普通 2:困难 ";
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
        cout << "对不起，您撞到墙了。游戏结束。";
    } else if (EndGameStatus == 2) {
        cout << "对不起，您咬到自己了。游戏结束。";
    } else if (EndGameStatus == 3) {
        cout << "您主动结束了游戏。";
    } else {
        cout << "未知错误";
    }
    Pos(COLS / 3, ROWS / 3 + 1);
    cout << "您的得分是：" << Para.score << endl;
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
    srand((unsigned)time(NULL));  //为了防止每次产生的随机数相同，种子设置为time
    InitMap();
    Snake.InitSnake();
    Food.CreateFood();
    cin.sync();
    RunGame();
}
