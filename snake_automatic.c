//
//  main.c
//  snake
//
//  Created by Yiji Liao on 2014-12-30
//  Copyright (c) 2014 Sun Yat-sen University. All rights reserved.
//
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAP_MAX_SIZE 102
#define SNAKE_MAX_LENGTH 500
#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define SNAKE_FOOD '$'
#define WALL_CELL '*'

//snake stepping: dy = -1(up),1(down); dx = -1(left),1(right),0(no move)
void snakeMove(int, int);
//put a food randomized on a blank cell
void put_money(void);
//out cells of the grid
void output(void);
//outs when gameover
void gameover(void);
//find the position of the snake
void find_snake(int, int, int);
//find where to go next
char whereGoNext(int, int, int, int);
//calculate the absolute value
int myabs(int);

char map[MAP_MAX_SIZE][MAP_MAX_SIZE];
int v[MAP_MAX_SIZE][MAP_MAX_SIZE];

// define vars for snake, notice name of vars in C
int snakeX[SNAKE_MAX_LENGTH];     //蛇身和蛇头的坐标 
int snakeY[SNAKE_MAX_LENGTH];
int snakeLength;

int n, m, foodX, foodY;

const int cx[4] = {-1, 1, 0, 0};
const int cy[4] = {0, 0, -1, 1};

int main() {
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    int i, j, preTime, curTime, headX, headY;
    char ch;
    srand((unsigned)time(NULL));
    snakeLength = 0;
    i = 0;
    foodX = foodY = -1;
    while (fscanf(fp, "%c", &ch) != EOF) {
        j = 0;
        if (ch == SNAKE_BODY || ch == SNAKE_HEAD) snakeLength++;
        if (ch == SNAKE_HEAD) headX = j, headY = i;
        if (ch == SNAKE_FOOD) foodX = j, foodY = i;
        map[i][j] = ch;
        j++;
        while (fscanf(fp, "%c", &ch) != EOF) {
            if (ch == '\n') break;
            if (ch == SNAKE_BODY || ch == SNAKE_HEAD) snakeLength++;
            if (ch == SNAKE_HEAD) headX = j, headY = i;
            if (ch == SNAKE_FOOD) foodX = j, foodY = i;
            map[i][j] = ch;
            j++;
        }
        i++;
    }
    fclose(fp);
    n = i; m = j;
    memset(v, 0, sizeof(v));
    find_snake(headX, headY, snakeLength-1);
    output();
    preTime = clock();
    ch = 'K';
    while (ch != 'N') {
        curTime = clock();
        if (curTime-preTime < 1000) continue;
        preTime = curTime;
        if (foodX == -1) {
            put_money();
            output();
            continue;
        }
        ch = whereGoNext(snakeX[snakeLength-1], snakeY[snakeLength-1], foodX, foodY);
        switch (ch) {
            case 'A' : snakeMove(cx[0], cy[0]); break;
            case 'D' : snakeMove(cx[1], cy[1]); break;
            case 'W' : snakeMove(cx[2], cy[2]); break;
            case 'S' : snakeMove(cx[3], cy[3]); break;
        }
        if (ch != 'N') output();
    }
    gameover();
    return 0;
}

void find_snake(int x, int y, int p) {
    int i, nx, ny;
    snakeX[p] = x; snakeY[p] = y;
    v[y][x] = 1;
    for (i = 0; i < 4; i++) {
        nx = x+cx[i]; ny = y+cy[i];
        if (nx > 0 && nx < m-1 && ny > 0 && ny < n-1 && !v[ny][nx] &&
        map[ny][nx] == 'X')
            find_snake(nx, ny, p-1);
    }
}

int myabs(int x) {
    if (x < 0) return -x;
    else return x;
}

char whereGoNext(int Hx, int Hy, int Fx, int Fy) {
    char movable[4] = {'A', 'D', 'W', 'S'};
    int distance[4] = {0, 0, 0, 0};
    int i, nx, ny, dMin, p;
    dMin = 9999; p = -1;
    for (i = 0; i < 4; i++) {
        nx = Hx+cx[i]; ny = Hy+cy[i];
        if (map[ny][nx] != BLANK_CELL && map[ny][nx] != SNAKE_FOOD)
            distance[i] = 9999;
        else
            distance[i] = myabs(Fx-nx)+myabs(Fy-ny);
        if (dMin > distance[i]) dMin = distance[i], p = i;
    }
    if (p == -1) return 'N';
    else return movable[p];
}

void snakeMove(int dx, int dy) {
    int i, j, new_sankeX, new_sankeY;
    map[snakeY[snakeLength-1]][snakeX[snakeLength-1]] = SNAKE_BODY;
    new_sankeX = snakeX[snakeLength-1]+dx;
    new_sankeY = snakeY[snakeLength-1]+dy;
    if (map[new_sankeY][new_sankeX] == SNAKE_FOOD) {
        snakeLength++;
        foodX = foodY = -1;
    } else {
        map[snakeY[0]][snakeX[0]] = BLANK_CELL;
        for (i = 0; i < snakeLength-1; i++) {
            snakeX[i] = snakeX[i+1];
            snakeY[i] = snakeY[i+1];
        }
    }
    snakeX[snakeLength-1] = new_sankeX;
    snakeY[snakeLength-1] = new_sankeY;
    map[snakeY[snakeLength-1]][snakeX[snakeLength-1]] = SNAKE_HEAD;
}

void put_money() {
    int Fx, Fy;
    Fx = rand()%10+1;
    Fy = rand()%10+1;
    while (map[Fy][Fx] != BLANK_CELL) {
        Fx = rand()%10+1;
        Fy = rand()%10+1;
    }
    map[Fy][Fx] = SNAKE_FOOD;
    foodX = Fx; foodY = Fy;
}

void output() {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            printf("%c", map[i][j]);
        printf("\n");
    }
}

void gameover() {
    printf("The snake can't move!\n");
    printf("Game Over!!!\n");
    system("pause");
}
