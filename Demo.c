#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include "Panel.h"
#include "Snake.h"
#include "Demo.h"
 
WINDOW *PanelCount;
WINDOW *PanelGame;

PosHead SnakePos;
FoodPos Food;
int DirX = 1, DirY = 0;
int Count = 0;
int TimerM = 4, TimerH = 4;

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));
    Food = malloc(sizeof(struct Food));
    
    Inital();
    PanelSet();
    
    CreateFood();
    CreateLinkList();
    signal(SIGALRM, MoveSnake);
    SetTrick();
    KeyResponse();
    
    Destroy();
    
    return 0;
}

// create the food and display it
void CreateFood(){
    do{
        Food->y = (rand()%(HGAME-2)) + 1;
        Food->x = (rand()%(WGAME-2)) + 1;
    }while(mvwinch(PanelGame, Food->y, Food->x) == SNAKEUP || mvwinch(PanelGame, Food->y, Food->x) == SNAKEDOWN || mvwinch(PanelGame, Food->y, Food->x) == SNAKELEFT || mvwinch(PanelGame, Food->y, Food->x) == SNAKERIGHT);
    mvwaddch(PanelGame, Food->y, Food->x, FOOD);
    wrefresh(PanelGame);
}

// create and initialize the sanke list
void CreateLinkList(){
    Snake SnakeBody = malloc(sizeof(struct Node));
    SnakePos = malloc(sizeof(struct Position));
    SnakeBody->y = HGAME/2;
    SnakeBody->x = WGAME/2;
    SnakeBody->Next = NULL;
    SnakeBody->Front = NULL;
    SnakePos->Head = SnakeBody;
    SnakePos->Tail = SnakeBody;
    mvwaddch(PanelGame, SnakeBody->y, SnakeBody->x, SNAKERIGHT);
    wrefresh(PanelGame);
}

// game over display
void GameOver(){
    Snake Temp = SnakePos->Head;
    Snake Temp2;
    mvwaddstr(PanelGame, HGAME/2, WGAME/2 - 4, "Game Over!");
    mvwaddstr(PanelGame, HGAME/2 + 1, WGAME/2 - 9, "Press any key to quit.");
    Count = 0;
    mvwprintw(PanelCount, 3, 1, "Count: %d", Count);
    wrefresh(PanelCount);
    wrefresh(PanelGame);
    while(Temp->Next != NULL){
        Temp2 = Temp;
        free(Temp2);
        Temp = Temp->Next;
    }
    getch();
    Destroy();
}

// insert the snake body
void InsertNode(int PosY, int PosX){
    Snake NewSnake = malloc(sizeof(struct Node));
    NewSnake->y = PosY;
    NewSnake->x = PosX;
    NewSnake->Front = NULL;
    NewSnake->Next = SnakePos->Head;
    SnakePos->Head->Front = NewSnake;
    SnakePos->Head = NewSnake;
}

// delete the node of snake in its tail
void DeleteNode(){
    Snake Temp = SnakePos->Tail;
    SnakePos->Tail = SnakePos->Tail->Front;
    SnakePos->Tail->Next = NULL;
    free(Temp);
}

// control the snake move and snake's direction and judge game over
void MoveSnake(){
    int Moved = 0;
    int LenghtAdd = 0;
    signal(SIGALRM, SIG_IGN);
    
    if(mvwinch(PanelGame, SnakePos->Head->y+DirY, SnakePos->Head->x+DirX) == SNAKEUP || mvwinch(PanelGame, SnakePos->Head->y+DirY, SnakePos->Head->x+DirX) == SNAKEDOWN || mvwinch(PanelGame, SnakePos->Head->y+DirY, SnakePos->Head->x+DirX) == SNAKELEFT || mvwinch(PanelGame, SnakePos->Head->y+DirY, SnakePos->Head->x+DirX) == SNAKERIGHT){
        GameOver();
    }
    
    if(TimerM > 0 && TimerH-- == 1){
        InsertNode(SnakePos->Head->y+DirY, SnakePos->Head->x+DirX);
        TimerH = TimerM;
        mvwprintw(PanelCount, 3, 1, "Count: %d             Speed: %d", Count, TimerM);
        wrefresh(PanelCount);
        Moved = 1;
    }
    
    if(Moved){
        if(Food->y == SnakePos->Head->y && Food->x == SnakePos->Head->x){
            Count++;
            mvwprintw(PanelCount, 3, 1, "Count: %d             Speed: %d", Count, TimerM);
            wrefresh(PanelCount);
            LenghtAdd = 1;
            CreateFood();
        }else if(SnakePos->Head->y == 0 && DirY == -1){
            SnakePos->Head->y = HGAME-2;
        }else if(SnakePos->Head->y == HGAME-1 && DirY == 1){
            SnakePos->Head->y = 1;
        }else if(SnakePos->Head->x == 0 && DirX == -1){
            SnakePos->Head->x = WGAME-2;
        }else if(SnakePos->Head->x == WGAME-1 && DirX == 1){
            SnakePos->Head->x = 1;
        }
        if(!LenghtAdd){
            mvwaddch(PanelGame, SnakePos->Tail->y, SnakePos->Tail->x, ' ');
            DeleteNode();
        }
        if(DirY == -1 && DirX == 0){
            mvwaddch(PanelGame, SnakePos->Head->y, SnakePos->Head->x, SNAKEUP);
        }else if(DirY == 1 && DirX == 0){
            mvwaddch(PanelGame, SnakePos->Head->y, SnakePos->Head->x, SNAKEDOWN);
        }else if(DirY == 0 && DirX == -1){
            mvwaddch(PanelGame, SnakePos->Head->y, SnakePos->Head->x, SNAKELEFT);
        }else if(DirY == 0 && DirX == 1){
            mvwaddch(PanelGame, SnakePos->Head->y, SnakePos->Head->x, SNAKERIGHT);
        }
        wrefresh(PanelGame);
    }
    signal(SIGALRM, MoveSnake);
}

// key response
void KeyResponse(){
    char ch;
    while((ch = getch()) != 'q'){
        if(ch == '.'){
            if(TimerM > 1){
                TimerM--;
            }
        }else if(ch == ','){
            if(TimerM < 8){
                TimerM++;
            }
        }else if(ch == 'w'){
            if(DirY != 1 && DirX != 0){
                DirY = -1;
                DirX = 0;
            }
        }else if(ch == 's'){
            if(DirY != -1 && DirX != 0){
                DirY = 1;
                DirX = 0;
            }
        }else if(ch == 'a' && DirY != 0){
            if(DirX != 1){
                DirY = 0;
                DirX = -1;
            }
        }else if(ch == 'd' && DirY != 0){
            if(DirX != -1){
                DirY = 0;
                DirX = 1;
            }
        }
    }
}

// set the alrm clock's time
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

// initialize the panel setting
void Inital(){
    initscr();
    cbreak();
    noecho();
    nonl();
    curs_set(0);
    refresh();
}

// panel setting
void PanelSet(){
    int CountY, CountX;
    int GameY, GameX;
    CountY = 1;
    CountX = (COLS - WCOUNT) / 2;
    GameY = HCOUNT + 2;
    GameX = (COLS - WGAME) / 2;
    
    PanelCount = newwin(HCOUNT, WCOUNT, CountY, CountX);
    PanelGame = newwin(HGAME, WGAME, GameY, GameX);
    box(PanelCount, 0, 0);
    box(PanelGame, 0, 0);
    mvwaddstr(PanelCount, 1, 1, VERSION);
    mvwaddstr(PanelCount, 2, 1, INTRODUCTION);
    mvwprintw(PanelCount, 3, 1, "Count: %d             Speed: %d", Count, TimerM);
    wrefresh(PanelCount);
    wrefresh(PanelGame);
}

// distroy the panel memorize
void Destroy(){
    delwin(PanelCount);
    delwin(PanelGame);
    endwin();
}