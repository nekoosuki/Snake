#ifndef __SNAKE_HPP__
#define __SNAKE_HPP__

#include <Windows.h>

#include <ctime>
#include <iostream>

using namespace std;

#define ROWS 30
#define COLS 100
#define U 1
#define D 2
#define L 3
#define R 4  //蛇的状态，U：上 ；D：下；L:左 R：右

//设定坐标
void Pos(int x, int y);

class para;
class snake;
class food;
class gameBase;
class gameMo1;
class gameMo2;

class para {
   public:
    int diff, sleeptime, score, add, dir, mode;
    bool FoodEaten;
    para() {
        diff = sleeptime = score = add = dir = mode = 0;
        FoodEaten = false;
    }
};

class snake {
   protected:
    int x, y;

   public:
    //链表结构 指向下一节蛇身
    snake* next;
    snake() {
        x = y = 0;
        next = NULL;
    }
    snake(int x, int y) {
        this->x = x;
        this->y = y;
        next = NULL;
    }
    //判断坐标重叠，用于判断吃食物和防止食物生成在蛇的位置
    friend bool EatFood(food Food, snake Snake);
    //初始化蛇
    void InitSnake();
    //判断蛇咬到自己
    void BiteSelf();
    //判断蛇撞到边界
    void HitWall();
    //判断移动后蛇的位置
    snake* SnakeMove();
    //加速F1
    void SpeedUp();
    //减速F2
    void SpeedDown();
    //输出整条蛇
    void PrintSnake(snake* nexthead);
};

class food {
   protected:
    int x, y;

   public:
    food() { x = y = 0; }
    food(int x, int y) {
        this->x = x;
        this->y = y;
    }
    friend bool EatFood(food Food, snake Snake);
    //创建食物
    void CreateFood();
};

//抽象类
class gameBase {
   public:
    snake Snake;
    //链表结构 指向蛇头
    snake* head;
    food Food;
    para Para;
    gameBase() {}
    virtual void RunGame() = 0;
    //初始化地图
    void InitMap();
    virtual void InitGame() = 0;
    virtual void EndGame(int EndGameStatus) = 0;
    virtual void GameEntry() = 0;
    //通过步时获取加分
    int GetAdd(int sleeptime);
    //暂停游戏
    void Pause();
    //结束一局游戏后删除全部对象
    void DeleteGame();
};

//普通模式
class gameMo1 : public gameBase {
   public:
    gameMo1() {}
    //游戏运行时的函数
    void RunGame();
    //初始化游戏参数 说明页面
    void InitGame();
    //结束游戏页面
    void EndGame(int EndGameStatus);
    //初始化游戏
    void GameEntry();
};

//寻找真食物模式
class gameMo2 : public gameBase {
   public:
    //是否生成过假食物
    bool fake;
    gameMo2() {}
    //游戏运行时的函数
    void RunGame();
    //初始化游戏参数 说明页面
    void InitGame();
    //结束游戏页面
    void EndGame(int EndGameStatus);
    //初始化游戏
    void GameEntry();
};

#endif