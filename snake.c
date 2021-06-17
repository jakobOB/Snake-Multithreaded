//
// Created by marson on 9/7/20.
//
#include "snake.h"
#include <stdlib.h>


WINDOW *game_window;
char game_map [MAP_LENGTH][MAP_WIDTH];

pthread_t user_snake_tid;
pthread_t enemy_snake_tid[NUMBER_ENEMIES];
pthread_t chest_tid;

time_t time1;

position user_snake_pos = {MAP_WIDTH / 2, MAP_LENGTH - 2,MAP_WIDTH / 2 + 1, MAP_LENGTH - 2,MAP_WIDTH / 2 + 2, MAP_LENGTH - 2};




int lifepoints = 100;
int points = 0;

int chest_collected_flag = 0;

void *snake()
{
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  char direction = 'l';
  char prev_direction = 'l';

  game_map[user_snake_pos.y_f_][user_snake_pos.x_f_] = (char) USER_SNAKE;
  game_map[user_snake_pos.y_m_][user_snake_pos.x_m_] = (char) USER_SNAKE;
  game_map[user_snake_pos.y_b_][user_snake_pos.x_b_] = (char) USER_SNAKE;

  refresh_map();
  int c;
  keypad (stdscr, TRUE);
  noecho();
  timeout(1);
  while (true)
  {
    c = getch();
    switch (c)
    {
      case 's':
        direction = 'd';
        break;
      case 'w':
        direction = 'u';
        break;
      case 'a':
        direction = 'l';
        break;
      case 'd':
        direction = 'r';
        break;
      default:
        break;
    }
    if(c == 'q')
    {
      lifepoints = 0;
      continue;
    }
    int counter = 10000;
    while(counter > 0)
    {
      if(game_map[user_snake_pos.y_f_][user_snake_pos.x_f_] == (char) ENEMY_SNAKE ||
         game_map[user_snake_pos.y_m_][user_snake_pos.x_m_] == (char) ENEMY_SNAKE||
         game_map[user_snake_pos.y_b_][user_snake_pos.x_b_] == (char) ENEMY_SNAKE )
      {
        lifepoints = 0;
      }
      counter--;
    }
    usleep(100000);
    if((game_map[user_snake_pos.y_f_][user_snake_pos.x_f_] == (char) CHEST_A ||
        game_map[user_snake_pos.y_m_][user_snake_pos.x_m_] == (char) CHEST_A||
        game_map[user_snake_pos.y_b_][user_snake_pos.x_b_] == (char) CHEST_A) )
    {
      pthread_cancel(chest_tid);
      chest_collected_flag = 1;
    }
    moveSnake(&direction, &prev_direction, &user_snake_pos, USER_SNAKE);

  }
}
void *enemySnakes(void* params)
{
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  parameters *t_params = params;
  unsigned char position_x_f = t_params->pos_x_;
  unsigned char position_y_f = t_params->pos_y_;
  unsigned char enemy_type = t_params->type_;

  free(params);

  position enemy_snake_pos = {position_x_f, position_y_f, position_x_f +1, position_y_f, position_x_f +2, position_y_f};

  game_map[enemy_snake_pos.y_f_][enemy_snake_pos.x_f_] = (char) enemy_type;
  game_map[enemy_snake_pos.y_m_][enemy_snake_pos.x_m_] = (char) enemy_type;
  game_map[enemy_snake_pos.y_b_][enemy_snake_pos.x_b_] = (char) enemy_type;

  int nr_direction = 0;
  char prev_direction = 'l';

  while(lifepoints > 0)
  {
    usleep(100000);
    int counter = 10000;
    while(counter > 0)
    {
      if(game_map[user_snake_pos.y_f_][user_snake_pos.x_f_] == (char) enemy_type ||
         game_map[user_snake_pos.y_m_][user_snake_pos.x_m_] == (char) enemy_type||
         game_map[user_snake_pos.y_b_][user_snake_pos.x_b_] == (char) enemy_type )
      {
        lifepoints = 0;
        break;
      }
      counter--;
    }
    if (rand() % 5 == 0)
    {
      nr_direction = rand() % 4;
    }
    char direction;
    switch(nr_direction)
    {
    case 0:
      direction = 'l';
      moveSnake(&direction, &prev_direction,&enemy_snake_pos,(char) enemy_type);
      break;
    case 1:
      direction = 'r';
      moveSnake(&direction, &prev_direction,&enemy_snake_pos,(char) enemy_type);
      break;
    case 2:
      direction = 'u';
      moveSnake(&direction, &prev_direction,&enemy_snake_pos,(char) enemy_type);
      break;
    case 3:
      direction = 'd';
      moveSnake(&direction, &prev_direction,&enemy_snake_pos, (char)enemy_type);
      break;
    default:
      break;
    }
  }
  return (void*)999;
}


void init_enemies(int pid_pos, unsigned char pos_x_, unsigned char pos_y_,
                  unsigned char type_ )
{
    parameters *t_parameter = (parameters*)malloc(sizeof(parameters));
    (*t_parameter).pos_x_ = pos_x_;
    (*t_parameter).pos_y_ = pos_y_;
    (*t_parameter).type_ = type_;
    pthread_create(&enemy_snake_tid[pid_pos], NULL, enemySnakes, t_parameter);
}
void *placeChests()
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  unsigned char position_x = getRandPosX();
  unsigned char position_y = getRandPosY();

  while(1)
  {
    game_map[position_y][position_x] = (char) CHEST_A;
  }
}


int main()
{
  void* rvalue_user_snake = 0;
  void* rvalue_enemies[NUMBER_ENEMIES];
  void* rvalue_chest = 0;

  init_map();

  pthread_create(&user_snake_tid, NULL, snake, NULL);

  srand((unsigned int) time(&time1));


  for (int i = 0; i < NUMBER_ENEMIES; i++)
  {
    unsigned char pos_x = getRandPosX();
    unsigned char pos_y = getRandPosY();

    init_enemies(i, pos_x, pos_y,ENEMY_SNAKE);
    if(i == 1)
      refresh_map();
  }

  pthread_create(&chest_tid, NULL, placeChests, NULL);

  while (lifepoints > 0)
  {
      usleep(10000);

      if (chest_collected_flag) {
          chest_collected_flag = 0;
          pthread_join(chest_tid, &rvalue_chest);
          pthread_create(&chest_tid, NULL, placeChests, NULL);
          points += POINTS_CHEST_A;
      }

      refresh_map();
  }
    pthread_cancel(user_snake_tid);
    pthread_join(user_snake_tid, &rvalue_user_snake);
    pthread_cancel(chest_tid);
    pthread_join(chest_tid, &rvalue_chest);
    for (int i = 0; i < NUMBER_ENEMIES; ++i) {
        pthread_cancel(enemy_snake_tid[i]);
        pthread_join(enemy_snake_tid[i], &rvalue_enemies[i]);
    }



  return end_game(rvalue_user_snake, rvalue_enemies, rvalue_chest);
}




