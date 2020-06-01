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
#define R 4  //ÉßµÄ×´Ì¬£¬U£ºÉÏ £»D£ºÏÂ£»L:×ó R£ºÓÒ

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
   public:
    snake* next;
    int x, y;
    snake() {
        x = y = 0;
        next = NULL;
    }
    snake(int x, int y) {
        this->x = x;
        this->y = y;
        next = NULL;
    }
    void InitSnake();
    void BiteSelf();
    void HitWall();
    snake* SnakeMove();
    void SpeedUp();
    void SpeedDown();
    void PrintSnake(snake* nexthead);
};

class food {
   public:
    int x, y;
    food() { x = y = 0; }
    food(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void CreateFood();
};

class gameBase {
   public:
    snake Snake;
    snake* head;
    food Food;
    para Para;
    gameBase() {}
    virtual void RunGame() = 0;
    void InitMap();
    virtual void InitGame() = 0;
    virtual void EndGame(int EndGameStatus) = 0;
    virtual void GameEntry() = 0;
    void Pause();
    void DeleteGame();
};

class gameMo1 : public gameBase {
   public:
    gameMo1() {}
    void RunGame();
    void InitGame();
    void EndGame(int EndGameStatus);
    void GameEntry();
};

class gameMo2 : public gameBase {
   public:
    bool fake;
    gameMo2() {}
    void RunGame();
    void InitGame();
    void EndGame(int EndGameStatus);
    void GameEntry();
};

#endif