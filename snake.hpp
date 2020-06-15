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
#define R 4  //�ߵ�״̬��U���� ��D���£�L:�� R����

//�趨����
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
    //����ṹ ָ����һ������
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
    //�ж������ص��������жϳ�ʳ��ͷ�ֹʳ���������ߵ�λ��
    friend bool EatFood(food Food, snake Snake);
    //��ʼ����
    void InitSnake();
    //�ж���ҧ���Լ�
    void BiteSelf();
    //�ж���ײ���߽�
    void HitWall();
    //�ж��ƶ����ߵ�λ��
    snake* SnakeMove();
    //����F1
    void SpeedUp();
    //����F2
    void SpeedDown();
    //���������
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
    //����ʳ��
    void CreateFood();
};

//������
class gameBase {
   public:
    snake Snake;
    //����ṹ ָ����ͷ
    snake* head;
    food Food;
    para Para;
    gameBase() {}
    virtual void RunGame() = 0;
    //��ʼ����ͼ
    void InitMap();
    virtual void InitGame() = 0;
    virtual void EndGame(int EndGameStatus) = 0;
    virtual void GameEntry() = 0;
    //ͨ����ʱ��ȡ�ӷ�
    int GetAdd(int sleeptime);
    //��ͣ��Ϸ
    void Pause();
    //����һ����Ϸ��ɾ��ȫ������
    void DeleteGame();
};

//��ͨģʽ
class gameMo1 : public gameBase {
   public:
    gameMo1() {}
    //��Ϸ����ʱ�ĺ���
    void RunGame();
    //��ʼ����Ϸ���� ˵��ҳ��
    void InitGame();
    //������Ϸҳ��
    void EndGame(int EndGameStatus);
    //��ʼ����Ϸ
    void GameEntry();
};

//Ѱ����ʳ��ģʽ
class gameMo2 : public gameBase {
   public:
    //�Ƿ����ɹ���ʳ��
    bool fake;
    gameMo2() {}
    //��Ϸ����ʱ�ĺ���
    void RunGame();
    //��ʼ����Ϸ���� ˵��ҳ��
    void InitGame();
    //������Ϸҳ��
    void EndGame(int EndGameStatus);
    //��ʼ����Ϸ
    void GameEntry();
};

#endif