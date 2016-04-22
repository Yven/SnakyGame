#define WGAME        55
#define HGAME        20
#define WCOUNT       55
#define HCOUNT       5
#define SPEED        50
#define SNAKEUP      '^'
#define SNAKEDOWN    'V'
#define SNAKERIGHT   '>'
#define SNAKELEFT    '<'
#define FOOD         '*'
#define VERSION      "version: 0.2.1                 Author:Yven"
#define INTRODUCTION "wasd=>direction ,.=>speed q=>quit p=>pause"

struct Node;
struct Position;
struct Food;
typedef struct Node* Snake;
typedef struct Position* PosHead;
typedef struct Food* FoodPos;

struct Node{
    int y;
    int x;
    struct Node* Next;
    struct Node* Front;
};

struct Position{
    Snake Head;
    Snake Tail;
};

struct Food{
    int y;
    int x;
};

void CreateFood();
void CreateLinkList();
void GameOver();
void InsertNode(int PosX, int PosY);
void DeleteNode();
void MoveSnake();
void KeyResponse();
int SetTrick();
void Inital();
void PanelSet();
void Destroy();