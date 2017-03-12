//
//  main.c
//  snake
//
//  Created by Yiji Liao on 2014-12-30
//  Copyright (c) 2014 Sun Yat-sen University. All rights reserved.
//
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SNAKE_MAX_LENGTH 20
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
//check whether the game is over
int game_is_over(void);

char map[12][12]=
    {"************",          //初始状态 
    "*XXXXH     *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "************"};

// define vars for snake, notice name of vars in C
int snakeX[SNAKE_MAX_LENGTH]={1,2,3,4,5};     //蛇身和蛇头的坐标 
int snakeY[SNAKE_MAX_LENGTH]={1,1,1,1,1};
int snakeLength = 5;

int main() {
    char ch;
    srand((unsigned)time(NULL));
    output();
    while (!game_is_over()) {
        printf("Please input a letter(A/D/W/S/F):\n");
        printf("A: left       D: right\n");
        printf("W: up         S: down\n");
        printf("F: put a food randomized\n");
        scanf("%c", &ch);
        while (ch != 'A' && ch != 'D' && ch != 'W' && ch != 'S' && ch != 'F')
            scanf("%c", &ch);
        switch (ch) {
            case 'A' : snakeMove(-1, 0); break;
            case 'D' : snakeMove(1, 0); break;
            case 'W' : snakeMove(0, -1); break;
            case 'S' : snakeMove(0, 1); break;
            case 'F' : put_money(); break;
        }
        output();
    }
    gameover();
    return 0;
}

int game_is_over() {
    int i;
    if (snakeX[snakeLength-1] == 0 || snakeX[snakeLength-1] == 11 ||
        snakeY[snakeLength-1] == 0 || snakeY[snakeLength-1] == 11) return 1;
    for (i = 0; i < snakeLength-1; i++)
        if (snakeX[i] == snakeX[snakeLength-1] &&
            snakeY[i] == snakeY[snakeLength-1]) return 1;
    return 0;
}

void snakeMove(int dx, int dy) {
    int i, j, new_sankeX, new_sankeY;
    map[snakeY[snakeLength-1]][snakeX[snakeLength-1]] = SNAKE_BODY;
    new_sankeX = snakeX[snakeLength-1]+dx;
    new_sankeY = snakeY[snakeLength-1]+dy;
    if (map[new_sankeY][new_sankeX] == SNAKE_FOOD) {
        snakeLength++;
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
    int foodX, foodY;
    foodX = rand()%10+1;
    foodY = rand()%10+1;
    while (map[foodY][foodX] != BLANK_CELL) {
        foodX = rand()%10+1;
        foodY = rand()%10+1;
    }
    map[foodY][foodX] = SNAKE_FOOD;
}

void output() {
    int i, j;
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 12; j++)
            printf("%c", map[i][j]);
        printf("\n");
    }
}

void gameover() {
    printf("Game Over!!!\n");
    system("pause");
}
