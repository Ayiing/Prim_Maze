/*61=--> wall,42*-->path */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define size 20
/*  size >=2,
    such as:
    0        not funny
*/
typedef struct node{
    int x;
    int y;
    int lastDir;//come from where
    struct node *next;
}queue;
int putWalls(char (*maze)[size],int x,int y,int (*adjWalls)[2],int len)//put the walls around the (x,y) into arr1,len-->the length of arr1,return current length
{
    if(x>=1)
        if(!maze[x-1][y]){
            // adjacent wall
            adjWalls[len][0]=x-1;
            adjWalls[len][1]=y;
            len++;
        }
    if(y>=1)
        if(!maze[x][y-1]){
            adjWalls[len][0]=x;
            adjWalls[len][1]=y-1;
            len++;
        }
    if(x<size-1)
        if(!maze[x+1][y]){
            adjWalls[len][0]=x+1;
            adjWalls[len][1]=y;
            len++;
        }
    if(y<size-1)
        if(!maze[x][y+1]){
            adjWalls[len][0]=x;
            adjWalls[len][1]=y+1;
            len++;
        }
    return len;
}
int checkWall(char (*maze)[size],int x,int y)//are there more than 2 paths around (x,y)?
{
    int num=0;
    if(x>=1)
        if(maze[x-1][y])
            num++;
    if(y>=1)
        if(maze[x][y-1])
            num++;
    if(x<size-1)
        if(maze[x+1][y])
            num++;
    if(y<size-1)
        if(maze[x][y+1])
            num++;
    return num;
}
void printMaze(char (*maze)[size])
{
    for(int i=0;i<size;i++){
        putchar('\t');
        for(int j=0;j<size;j++){
            if(!maze[i][j]){
                //wall
                //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                printf("%c ",61);
                continue;
            }
            //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            printf("%c ",maze[i][j]);
        }
        printf("\n");
    }

}
void primMaze(char (*maze)[size],int x0,int y0,int endx,int endy)
{
    int adjWalls[size*size][2]={0};
    maze[x0][y0]=42;
    int len=putWalls(maze,x0,y0,adjWalls,0);
    while(1){
        int randomWall=rand()%len;
        x0=adjWalls[randomWall][0],y0=adjWalls[randomWall][1];
        if(checkWall(maze,x0,y0)==1){
            //just only one path adjacent to the wall
                maze[x0][y0]=42;
                len=putWalls(maze,x0,y0,adjWalls,len);
        }
        // remove the wall in the adjWalls
        adjWalls[randomWall][0]=adjWalls[len-1][0];
        adjWalls[randomWall][1]=adjWalls[len-1][1];
        len--;
        if(x0==endx&&y0==endy){// this means : there may be two paths toward the exit ,
                            // but there will not   in the maze;
            maze[x0][y0]='+';
            break;
        }
//        system("cls");
//        printMaze(maze);
//        Sleep(100);
    }
}
int DFS(char (*maze)[size],int x0,int y0,int endx,int endy,int lastDir)
{
    if(x0<0||y0<0||x0>=size||y0>=size)
        return 0;
    if(!maze[x0][y0])
        return 0;
    // arrive a new place
    maze[x0][y0]='0';
    system("cls");

//    //获取默认标准显示缓冲区句柄
//    HANDLE hOutput;
//    COORD coord={0,0};
//    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
//
//    //创建新的缓冲区
//    HANDLE hOutBuf = CreateConsoleScreenBuffer(
//        GENERIC_READ | GENERIC_WRITE,
//        FILE_SHARE_READ | FILE_SHARE_WRITE,
//        NULL,
//        CONSOLE_TEXTMODE_BUFFER,
//        NULL
//    );
//
//    //设置新的缓冲区为活动显示缓冲
//    SetConsoleActiveScreenBuffer(hOutBuf);
//
//    //隐藏两个缓冲区的光标
//    CONSOLE_CURSOR_INFO cci;
//    cci.bVisible=0;
//    cci.dwSize=1;
//    SetConsoleCursorInfo(hOutput, &cci);
//    SetConsoleCursorInfo(hOutBuf, &cci);
//
//    //双缓冲处理显示
//    DWORD bytes=0;
//    char data[800];
//
//    ReadConsoleOutputCharacterA(hOutput, data, 800, coord, &bytes);
//    WriteConsoleOutputCharacterA(hOutBuf, data, 800, coord, &bytes);


    printMaze(maze);
    Sleep(500);
    if((x0==endx)&&(y0==endy))
        return 1;
    int num=4,dirPass[4]={0};
    /* up--0 down--3 left--1 right--2*/
    dirPass[3-lastDir]=1;//come from where
    while(1){
        int dir=rand()%num;
        char temp;
        if(dirPass[dir])
            continue;
        switch(dir){
            case 0:
                dirPass[0]=1;
                temp=maze[x0-1][y0];
                if(DFS(maze,x0-1,y0,endx,endy,0))
                    exit(2);
                maze[x0-1][y0]=temp;
                break;
            case 1:
                dirPass[1]=1;
                temp=maze[x0][y0-1];
                if(DFS(maze,x0,y0-1,endx,endy,1))
                    exit(2);
                maze[x0][y0-1]=temp;
                break;
            case 2:
                dirPass[2]=1;
                temp=maze[x0][y0+1];
                if(DFS(maze,x0,y0+1,endx,endy,2))
                    exit(2);
                maze[x0][y0+1]=temp;
                break;
            case 3:
                dirPass[3]=1;
                temp=maze[x0+1][y0];
                if(DFS(maze,x0+1,y0,endx,endy,3))
                    exit(2);
                maze[x0+1][y0]=temp;
                break;
        }
        if(dirPass[0]&&dirPass[1]&&dirPass[2]&&dirPass[3])
            break;
    }
    return 0;
}
queue *findTail(queue*tail)//return tail--> the last node in the queue
{
    if(!tail)
        return NULL;
    while(tail->next)
        tail=tail->next;
    return tail;
}
void inQueue(queue *tail,char (*maze)[size],int x0,int y0,int lastDir)
{
    //put the paths around the (x0,y0) into the tail of the queue
    /* up--0 down--3 left--1 right--2*/
    int i=0;
    while(i<4){
        if(lastDir+i==3){
            i++;
            continue;
        }
        switch(i){
        case 0:
            if(x0>=1&&maze[x0-1][y0]){
                tail->next=(queue*)malloc(sizeof(queue));
                tail=tail->next,tail->next=NULL;
                tail->x=x0-1,tail->y=y0;
                tail->lastDir=0;
            }
            break;
        case 1:
            if(y0>=1&&maze[x0][y0-1]){
                tail->next=(queue*)malloc(sizeof(queue));
                tail=tail->next,tail->next=NULL;
                tail->x=x0,tail->y=y0-1;
                tail->lastDir=1;
            }
            break;
        case 2:
            if(y0<size-1&&maze[x0][y0+1]){
                tail->next=(queue*)malloc(sizeof(queue));
                tail=tail->next,tail->next=NULL;
                tail->x=x0,tail->y=y0+1;
                tail->lastDir=2;
            }
            break;
        case 3:
            if(x0<size-1&&maze[x0+1][y0]){
                tail->next=(queue*)malloc(sizeof(queue));
                tail=tail->next,tail->next=NULL;
                tail->x=x0+1,tail->y=y0;
                tail->lastDir=3;
            }
            break;
        }
        i++;
    }
}
void BFS(char (*maze)[size],int x0,int y0,int endx,int endy)
{
    queue *head=(queue*)malloc(sizeof(queue)),*tail;
    head->x=x0,head->y=y0;
    head->lastDir=3;//nothing influence
    head->next=NULL;
    tail=head;
    while(head){
        maze[head->x][head->y]='0';
        system("cls");
        printMaze(maze);
        Sleep(100);
        if(head->x==endx&&head->y==endy)
            exit(2);
        inQueue(tail,maze,head->x,head->y,head->lastDir);
        tail=findTail(tail);
        queue *temp=head;
        head=head->next;
        free(temp);
    }
}
int main()
{

    char maze[size][size]={0};
    int x0=0,y0=0,endx=size-1,endy=size-1;
    srand((unsigned int)time(NULL));
    primMaze(maze,x0,y0,endx,endy);
    //printMaze(maze);
    DFS(maze,x0,y0,endx,endy,3);
    //system("pause");
    //BFS(maze,x0,y0,endx,endy);
    printf("Hello world!\n%c",22);
    return 0;
}
