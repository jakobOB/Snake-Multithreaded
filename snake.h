//
// Created by marson on 9/7/20.
//
#include <ncurses.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include "string.h"
#include <time.h>

#define MAP_WIDTH 50
#define MAP_LENGTH 50
#define EMPTY 1
#define USER_SNAKE 2
#define CHEST_A 3
#define ENEMY_SNAKE 6

#define GRASS 51
#define SNAKE_COLOR_USER 52
#define SNAKE_COLOR_ENEMY_1 53
#define CHEST_A_COLOR 54

#define POINTS_CHEST_A 2000
#define NUMBER_ENEMIES 10

#ifndef SNP_TUTOR_TEST_1_H
#define SNP_TUTOR_TEST_1_H

typedef struct
{
  unsigned char x_f_;
  unsigned char y_f_;
  unsigned char x_m_;
  unsigned char y_m_;
  unsigned char x_b_;
  unsigned char y_b_;
}position;

typedef struct
{
  unsigned char pos_x_;
  unsigned char pos_y_;
  unsigned char type_;
}parameters;

extern WINDOW *game_window;
extern char game_map[MAP_LENGTH][MAP_WIDTH];


extern int lifepoints;
extern int points;
extern pthread_t enemy_snake_tid[NUMBER_ENEMIES];
extern position user_snake_pos;
extern pthread_t user_snake_tid;
extern pthread_t chest_tid;
extern time_t time1;


void init_map();

void refresh_map();

int end_game(void* rvalue, void** rvalues_enemies, void* rvalue_chest);


void* snake();

void* enemySnakes(void* params);

void* placeChests();


void init_enemies(int pid_pos, unsigned char pos_x_, unsigned char pos_y_,
                  unsigned char type_ );

void moveSnake(char* direction,char* prev_direction, position* snake_pos, char snake_type);

char getRandPosX();

char getRandPosY();


#endif
