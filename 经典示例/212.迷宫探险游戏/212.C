#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <graphics.h>

#define N 22
#define M 22
int bg[M][N];

void makebg(int, int);
void drawbg(int[][], int, int, int, int, int);
void drawman(int, int, int);
void rect(int, int, int, int);

void main()
{
    /* main() 开始 */
    int gdriver = VGA, gmode = VGAHI;
    int direc;
    char ch, ch2;
    int step, len, size, x, y, i, j;
    initgraph(&gdriver, &gmode, "c:\\tc");
Startgame:
    step = 20;
    len = 10;
    size = 20;
    x = 0;
    y = 0;
    i = 0;
    j = 0;
    makebg(M, N);
    cleardevice();
    setbkcolor(LIGHTBLUE);
    setwritemode(XOR_PUT);
    settextstyle(1, 0, 3);
    /*setcolor(GREEN);
    outtextxy(100, 180, "Press <Q> to quit");*/
    setcolor(RED);
    setfillstyle(LINE_FILL, RED);
    drawbg(bg, M, N, size, 0, 0);
    setcolor(WHITE);
    x += len; y += len;
    drawman(x, y, len);
    gotoxy(60, 3);
    printf("M-by Manual");
    gotoxy(60, 6);
    printf("C-by Computer");
    gotoxy(60, 9);
    printf("Please choice:");
    setcolor(WHITE);
    while (1)
    {
        ch = getch();
        if (ch == 'M' || ch == 'm')
        {
            /* 手动模式 */
            gotoxy(60, 3);
            printf("A-Left     ");
            gotoxy(60, 6);
            printf("D-Right      ");
            gotoxy(60, 9);
            printf("W-Up           ");
            gotoxy(60, 12);
            printf("S-Down");
            gotoxy(60, 15);
            printf("Q-Quit");
            while (1)
            {
                ch = getch();
                drawman(x, y, len);  // 先清除旧位置
                if (ch == 'Q' || ch == 'q')
                {
                    /* 按下Q键退出游戏 */
                    gotoxy(60, 18);
                    printf("You have press");
                    gotoxy(60, 19);
                    printf("Q key! Do you");
                    gotoxy(60, 20);
                    printf("Want to quit?");
                    gotoxy(60, 21);
                    printf("OK?[Y/N]");
                    while (1)
                    {
                        ch2 = getch();
                        if (ch2 == 'Y' || ch2 == 'y')
                        {
                            closegraph();
                            clrscr();
                            exit(0);
                        }
                        else if (ch2 == 'N' || ch2 == 'n')
                            goto Startgame;
                    }
                }
                switch (ch)
                {
                    case 'a':
                    case 'A':
                        if (j > 0 && bg[i][j - 1] == 0)
                        {
                            if (x > step)
                            {
                                x -= step;
                                j--;
                            }
                        }
                        break;
                    case 's':
                    case 'S':
                        if (i < M - 1 && bg[i + 1][j] == 0)
                        {
                            if (y < 479 - step)
                            {
                                y += step;
                                i++;
                            }
                        }
                        break;
                    case 'd':
                    case 'D':
                        if (j < N - 1 && bg[i][j + 1] == 0)
                        {
                            if (x < 639 - step)
                            {
                                x += step;
                                j++;
                            }
                        }
                        break;
                    case 'w':
                    case 'W':
                        if (i > 0 && bg[i - 1][j] == 0)
                        {
                            if (y > step)
                            {
                                y -= step;
                                i--;
                            }
                        }
                        break;
                    default:
                        break;
                }
                drawman(x, y, len);  // 重新绘制到新位置
                delay(800);
                if (i >= M - 1 && j >= N - 1)
                {
                    setcolor(LIGHTGREEN);
                    rectangle(450, 300, 620, 380);
                    setfillstyle(SOLID_FILL, LIGHTGRAY);
                    floodfill(480, 360, LIGHTGREEN);
                    settextstyle(0, 0, 1);
                    setcolor(MAGENTA);
                    outtextxy(455, 325, "You won the game!");
                    outtextxy(455, 355, "Press Q to quit...");
                    while (1)
                    {
                        ch2 = getch();
                        if (ch2 == 'Q' || ch2 == 'q')
                            break;
                    }
                    closegraph();
                    clrscr();
                    exit(0);
                }
            }
        } /* 手动模式结束 */

        else if (ch == 'C' || ch == 'c')
        {
            /* 计算机模式 */
            /* direc 表示当前的移动方向 */
            /* 0~3 分别表示 下、左、上、右 */
            gotoxy(60, 3);
            printf("Computer now");
            gotoxy(60, 6);
            printf("Run the maze");
            gotoxy(60, 9);
            printf("Automatically...");
            direc = 2;
            i = j = 0;
            while (i < M - 1 || j < N - 1)
            {
                delay(800);
                drawman(x, y, len);  // 先清除旧位置
                switch (direc)
                {
                    case 0:
                        /* 尝试向下、左、上、右移动 */
                        if (i < M - 1 && bg[i + 1][j] == 0)
                        {
                            y += step;
                            i++;
                            direc = 3;
                        }
                        else if (j > 0 && bg[i][j - 1] == 0)
                        {
                            x -= step;
                            j--;
                            direc = 0;
                        }
                        else if (i > 0 && bg[i - 1][j] == 0)
                        {
                            y -= step;
                            i--;
                            direc = 1;
                        }
                        else
                        {
                            x += step;
                            j++;
                            direc = 2;
                        }
                        break;
                    case 1:
                        if (j > 0 && bg[i][j - 1] == 0)
                        {
                            x -= step;
                            j--;
                            direc = 0;
                        }
                        else if (i > 0 && bg[i - 1][j] == 0)
                        {
                            y -= step;
                            i--;
                            direc = 1;
                        }
                        else if (j < N - 1 && bg[i][j + 1] == 0)
                        {
                            x += step;
                            j++;
                            direc = 2;
                        }
                        else
                        {
                            y += step;
                            i++;
                            direc = 3;
                        }
                        break;
                    case 2:
                        if (i > 0 && bg[i - 1][j] == 0)
                        {
                            y -= step;
                            i--;
                            direc = 1;
                        }
                        else if (j < N - 1 && bg[i][j + 1] == 0)
                        {
                            x += step;
                            j++;
                            direc = 2;
                        }
                        else if (i < M - 1 && bg[i + 1][j] == 0)
                        {
                            y += step;
                            i++;
                            direc = 3;
                        }
                        else
                        {
                            x -= step;
                            j--;
                            direc = 0;
                        }
                        break;
                    case 3:
                        if (j < N - 1 && bg[i][j + 1] == 0)
                        {
                            x += step;
                            j++;
                            direc = 2;
                        }
                        else if (i < M - 1 && bg[i + 1][j] == 0)
                        {
                            y += step;
                            i++;
                            direc = 3;
                        }
                        else if (j > 0 && bg[i][j - 1] == 0)
                        {
                            x -= step;
                            j--;
                            direc = 0;
                        }
                        else
                        {
                            y -= step;
                            i--;
                            direc = 1;
                        }
                        break;
                    default:
                        break;
                }
                drawman(x, y, len);  // 重新绘制到新位置
            }
            gotoxy(60, 12);
            printf("Finished!");
            gotoxy(60, 15);
            printf("Press any");
            gotoxy(60, 18);
            printf("Key to quit...");
            getch();
            closegraph();
            clrscr();
            exit(0);
        } /* 计算机模式结束 */
    }
} /* main() 结束 */

/* 绘制小人 */
void drawman(int x, int y, int len)
{
    int r = len / 4;
    rect(x - r, y - len, x + r, y - len + 2 * r);  // 绘制头部
    line(x, y - len + 2 * r, x, y);  // 绘制身体
    line(x - len, y, x + len, y);  // 绘制双臂
    line(x, y, x - len, y + len);  // 绘制左腿
    line(x, y, x + len, y + len);  // 绘制右腿
}

/* 绘制迷宫地图 */
void drawbg(int bg[][N], int a, int b, int size, int x, int y)
{
    int startx = x;
    int i, j;
    for (i = 0; i < a; i++)
    {
        for (j = 0; j < b; j++)
        {
            if (bg[i][j] == 1)
                rect(x, y, x + size - 1, y + size - 1);
            x += size;
        }
        x = startx;
        y += size;
    }
    rectangle(0, 0, size * b, size * a);
    line(0, 0, size, 0);
    line(0, 0, 0, size);
    line(size * b, size * (a - 1), size * b, size * a);
    line(size * (b - 1), size * a, size * b, size * a);
}

/* 绘制矩形 */
void rect(int x0, int y0, int x1, int y1)
{
    int i, j;
    for (i = x0; i <= x1; i++)
        line(i, y0, i, y1);
}

/* 生成迷宫地图 */
void makebg(int a, int b)
{
    int i, j;
    int ran;
    int direc;

    /* 初始化迷宫地图 */
    for (i = 0; i < a; i++)
        for (j = 0; j < b; j++)
            bg[i][j] = 1;

    /* 生成迷宫通路 */
    srand(time(0));  // 使用当前时间作为随机数种子
    i = j = 0;
    direc = 2;
    while (1)
    {
        bg[i][j] = 0;
        if (i >= M - 1 && j >= N - 1) break;
        ran = rand() % 4;  // 生成0到3之间的随机数
        if (ran == 0)
        {
            if (direc != 1 && i < a - 1)
            {
                i++;
                direc = 3;
            }
        }
        else if (ran == 1)
        {
            if (direc != 2 && j > 0)
            {
                j--;
                direc = 0;
            }
        }
        else if (ran == 2)
        {
            if (direc != 3 && i > 0)
            {
                i--;
                direc = 1;
            }
        }
        else
        {
            if (direc != 0 && j < b - 1)
            {
                j++;
                direc = 2;
            }
        }
    }

    /* 生成迷宫其余部分 */
    for (i = 0; i < a; i++)
    {
        for (j = 0; j < b; j++)
        {
            if (bg[i][j] == 1)
            {
                ran = rand() % 10;  // 生成0到9之间的随机数
                if (ran < 7) bg[i][j] = 0;  // 有70%的概率将该位置设为通路
            }
        }
    }
}
//重新提交一次 
