/**
1. 所有对窗口进行的操作都必须在initscr()函数中进行、
    包括读取全局变量的值

2. newwin(height, weight, Y, X);
    基本上所有的函数参数都是以Y，X顺序传入的 

3. 函数默认操作stdscr窗体变量
    在函数前加w能操作自定义的子窗体
*/

#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include "Demo.h"
 
WINDOW *PanelCount;
WINDOW *PanelGame;

PosHead SnakePos;
FoodPos Food;
int DirX = 1, DirY = 0;

// 生成并显示食物
void CreateFood(){
    Food->y = rand()%(HGAME-2) + 1;
    Food->x = rand()%(WGAME-2) + 1;
    while(mvwinch(PanelGame, Food->y, Food->x) == '@'){
        Food->y = rand()%(HGAME-2) + 1;
        Food->x = rand()%(WGAME-2) + 1;
    }
    mvwprintw(PanelGame, Food->y, Food->x, "*");
    wrefresh(PanelGame);
}

// 初始化蛇
void CreateLinkList(){
    Snake SnakeBody = malloc(sizeof(struct Node));
    SnakePos = malloc(sizeof(struct Position));
    SnakeBody->y = HGAME/2;
    SnakeBody->x = WGAME/2;
    SnakeBody->Next = NULL;
    SnakeBody->Front = NULL;
    SnakePos->Head = SnakeBody;
    SnakePos->Tail = SnakeBody;
    mvwprintw(PanelGame, SnakeBody->y, SnakeBody->x, "@");
    wrefresh(PanelGame);
}

// 游戏结束
void GameOver(){
    Snake Temp = SnakePos->Head;
    Snake Temp2;
    wclear(PanelGame);
    mvwaddstr(PanelGame, HGAME/2, WGAME/2, "Game Over!");
    wrefresh(PanelGame);
    while(Temp->Next != NULL){
        Temp2 = Temp;
        free(Temp2);
        Temp = Temp->Next;
    }
    getch();
    delwin(PanelCount);
    delwin(PanelGame);
    endwin();
}

// 添加蛇头节点
void InsertNode(int PosX, int PosY){
    Snake NewSnake = malloc(sizeof(struct Node));
    NewSnake->y = PosY;
    NewSnake->x = PosX;
    NewSnake->Front = NULL;
    NewSnake->Next = SnakePos->Head;
    SnakePos->Head = NewSnake;
}

// 删除蛇尾节点
void DeleteNode(){
    Snake Temp = SnakePos->Tail;
    SnakePos->Tail = SnakePos->Tail->Front;
    SnakePos->Tail->Next = NULL;
    free(Temp);
}

// 控制蛇方向
void MoveSnake(){
    int Length_Flag = 0;
    signal(SIGALRM, SIG_IGN);
    
    if((SnakePos->Head->x == WGAME-1 && DirX == 1)   
        || (SnakePos->Head->x == 1 && DirX == -1)  
        || (SnakePos->Head->y == 1 && DirY == -1)  
        || (SnakePos->Head->y == HGAME-1 && DirY == 1)){  
        GameOver();
    }

    if(mvinch(SnakePos->Head->y+DirY, SnakePos->Head->x+DirX) == '@')
        GameOver();
  
    InsertNode(SnakePos->Head->x+DirX, SnakePos->Head->y+DirY);
    
    if(SnakePos->Head->x == Food->x && SnakePos->Head->y == Food->y){
        Length_Flag = 1;
        CreateFood();
    }
    if(Length_Flag == 0){
        mvaddch(SnakePos->Tail->y, SnakePos->Tail->x, ' ');
        DeleteNode();
    }
    mvaddch(SnakePos->Head->y, SnakePos->Head->x, '@');
    wrefresh(PanelGame);

    signal(SIGALRM, MoveSnake);
    
    
    
    // int LenghtAdd = 0;
    // signal(SIGALRM, SIG_IGN);
    // 
    // if(mvwinch(PanelGame, SnakePos->Head->y+DirY, SnakePos->Head->x+DirX) == '@'){
    //     // 蛇身
    //     GameOver();
    // }
    // 
    // if(Food->y == SnakePos->Head->y+DirY && Food->x == SnakePos->Head->x+DirX){
    //     // 食物
    //     LenghtAdd = 1;
    //     CreateFood();
    // }else if(SnakePos->Head->y+DirY == 1){
    //     SnakePos->Head->y = HGAME-1;
    //     // 墙壁上
    // }else if(SnakePos->Head->y+DirY == HGAME-1){
    //     SnakePos->Head->y = 1;
    //     // 墙壁下
    // }else if(SnakePos->Head->x+DirX == 1){
    //     SnakePos->Head->x = WGAME-1;
    //     // 墙壁左
    // }else if(SnakePos->Head->x+DirX == WGAME-1){
    //     SnakePos->Head->x = 1;
    //     // 墙壁右
    // }
    // if(!LenghtAdd){
    //     mvwaddch(PanelGame, SnakePos->Tail->y, SnakePos->Tail->x, ' ');
    //     DeleteNode();
    // }
    // mvwaddch(PanelGame, SnakePos->Head->y+DirY, SnakePos->Head->x+DirX, '@');
    // InsertNode(SnakePos->Head->y+DirY, SnakePos->Head->x+DirX);
    // wrefresh(PanelGame);
    // 
    // signal(SIGALRM, MoveSnake);
}

// 键盘控制
void KeyResponse(){
    char ch;
    while((ch = getch()) != 'q'){
        if(ch == 'w'){
            if(DirY != 1){
                DirY = -1;
                DirX = 0;
            }
        }else if(ch == 's'){
            if(DirY != -1){
                DirY = 1;
                DirX = 0;
            }
        }else if(ch == 'a'){
            if(DirX != 1){
                DirY = 0;
                DirX = -1;
            }
        }else if(ch == 'd'){
            if(DirX != -1){
                DirY = 0;
                DirX = 1;
            }
        }
    }
}

// 设置闹钟时间间隔
int SetTrick(){
    struct itimerval trick;
    memset(&trick, 0, sizeof(struct itimerval)); 
    long Sec, uSec;
    Sec = SPEED / 1000;
    uSec = (SPEED % 1000) * 1000L;
    trick.it_value.tv_sec = Sec;
    trick.it_value.tv_usec = uSec;
    trick.it_interval.tv_sec = Sec;
    trick.it_interval.tv_usec = uSec;
    
    return setitimer(ITIMER_REAL, &trick, NULL);
}

// 初始化屏幕设置
void Inital(){
    initscr();
    cbreak();
    noecho();
    nonl();
    curs_set(0);
    refresh();
}

int main(int argc, char *argv[]) {
    int GameY, GameX;
    int CountY, CountX;
    int Count = 0;
    srand(time(NULL));
    
    // 初始化curses1
    Inital();
    
    CountY = 1;
    CountX = (COLS - WCOUNT) / 2;
    GameY = HCOUNT + 2;
    GameX = (COLS - WGAME) / 2;
    
    // 初始化计分面板和游戏窗体
    PanelCount = newwin(HCOUNT, WCOUNT, CountY, CountX);
    PanelGame = newwin(HGAME, WGAME, GameY, GameX);
    box(PanelCount, 0, 0);
    box(PanelGame, 0, 0);
    mvwprintw(PanelCount, 1, 1, "Press 'q' to quit.");
    mvwprintw(PanelCount, 2, 1, "Press 'w','a','s' and 'd' to control snake move.");
    mvwprintw(PanelCount, 3, 1, "Count: %d", Count);
    wrefresh(PanelCount);
    wrefresh(PanelGame);
    
    Food = malloc(sizeof(struct Food));
    
    // 游戏循环
    CreateFood();
    CreateLinkList();
    signal(SIGALRM, MoveSnake);
    SetTrick();
    KeyResponse();
    // while(1){
    //     mvwprintw(PanelGame, HGAME/2, WGAME/2, "hehe");
    // }
    
    // getch();

    // 清楚窗体内存，退出curses模式
    delwin(PanelCount);
    delwin(PanelGame);
    endwin();
    
    return 0;
}