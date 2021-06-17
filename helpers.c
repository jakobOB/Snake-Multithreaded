//
// Created by marson on 9/7/20.
//

#include "snake.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void init_map()
{

  for(int y = 0; y < MAP_LENGTH; y++)
  {
    for(int x = 0; x < MAP_WIDTH; x++)
    {
      game_map[y][x] = (char)EMPTY;
    }
  }
  initscr();

  start_color();
  init_pair(GRASS, COLOR_YELLOW, COLOR_GREEN);
  init_pair(SNAKE_COLOR_USER, COLOR_WHITE, COLOR_BLUE);
  init_pair(SNAKE_COLOR_ENEMY_1, COLOR_BLACK, COLOR_RED);
  init_pair(CHEST_A_COLOR, COLOR_BLACK, COLOR_MAGENTA);

  if(has_colors() != TRUE)
  {
    printf("ERROR: Your Screen does not support colors\n");
  }
  game_window = newwin(MAP_LENGTH, MAP_WIDTH, 0, 0);
}


void refresh_map()
{
  werase(game_window);

  mvwprintw(game_window, 1, 1, "ATTENTION THE SNAKE");
  mvwprintw(game_window, 1, 43, "v.0.1.1");
  mvwprintw(game_window, 2, 1, "POINTS: %07d\n",points);

  for (int y = 3; y < MAP_LENGTH; ++y)
  {
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
      if (game_map[y][x] != EMPTY)
      {
        if (game_map[y][x] == USER_SNAKE)
        {
          wattron(game_window,COLOR_PAIR(SNAKE_COLOR_USER));
          mvwaddch(game_window, y, x, 'S');
          wattroff(game_window,COLOR_PAIR(SNAKE_COLOR_USER));
        }
        else if (game_map[y][x] == ENEMY_SNAKE)
        {
          wattron(game_window,COLOR_PAIR(SNAKE_COLOR_ENEMY_1));
          mvwaddch(game_window, y, x, 'S');
          wattroff(game_window,COLOR_PAIR(SNAKE_COLOR_ENEMY_1));
        }
        else if (game_map[y][x] == CHEST_A)
        {
          wattron(game_window,COLOR_PAIR(CHEST_A_COLOR));
          mvwaddch(game_window, y, x, '#');
          wattroff(game_window,COLOR_PAIR(CHEST_A_COLOR));
        }
      }
      else
      {
        wattron(game_window,COLOR_PAIR(GRASS));
        mvwaddch(game_window, y, x, '\0');
        wattroff(game_window,COLOR_PAIR(GRASS));
      }
    }
  }
  wrefresh(game_window);
}


char getRandPosX()
{
  return rand()%30 + 5;
}

char getRandPosY()
{
  return rand()%30 + 5;
}

void moveSnake(char* direction, char* prev_direction, position* snake_pos, char snake_type)
{
  switch(*direction)
  {
  case 'l':
    if(*prev_direction == 'u' || *prev_direction == 'd')
    {
      //rotate into horizontal
      game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
      game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;

      game_map[++snake_pos->y_f_][--snake_pos->x_f_] = (char) snake_type;
      game_map[--snake_pos->y_b_][++snake_pos->x_b_] = (char) snake_type;
    }
    else
    {
      if(snake_pos->x_f_ < 1)
      {
        if(snake_type == USER_SNAKE)
        {
          lifepoints = 0;
          break;
        }
        else
        {
          //removing snake
          game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
          game_map[snake_pos->y_m_][snake_pos->x_m_] = (char) EMPTY;
          game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;

          //revoking on the opposite site of the map
          snake_pos->x_f_ = MAP_WIDTH - 3;
          snake_pos->x_m_ = MAP_WIDTH - 2;
          snake_pos->x_b_ = MAP_WIDTH - 1;
          game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) snake_type;
          game_map[snake_pos->y_m_][snake_pos->x_m_] = (char) snake_type;
          game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) snake_type;
          break;
        }
      }
      game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;
      game_map[snake_pos->y_f_][--snake_pos->x_f_] = (char) snake_type;
      game_map[snake_pos->y_m_][--snake_pos->x_m_] = (char) snake_type;
      game_map[snake_pos->y_b_][--snake_pos->x_b_] = (char) snake_type;
    }
    break;

  case 'r':
    if(*prev_direction == 'u' || *prev_direction == 'd')
    {
      //rotate into horizontal
      game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
      game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;

      game_map[++snake_pos->y_f_][--snake_pos->x_f_] = (char) snake_type;
      game_map[--snake_pos->y_b_][++snake_pos->x_b_] = (char) snake_type;
    }
    else
    {
      if(snake_pos->x_b_ > MAP_WIDTH - 2)
      {
        if(snake_type == USER_SNAKE)
        {
          lifepoints = 0;
          break;
        }
        else
        {
          //removing snake
          game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
          game_map[snake_pos->y_m_][snake_pos->x_m_] = (char) EMPTY;
          game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;

          //revoking on the opposite site of the map
          snake_pos->x_f_ = 1;
          snake_pos->x_m_ = 2;
          snake_pos->x_b_ = 3;
          game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) snake_type;
          game_map[snake_pos->y_m_][snake_pos->x_m_] = (char) snake_type;
          game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) snake_type;
          break;
        }
      }
      game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
      game_map[snake_pos->y_f_][++snake_pos->x_f_] = (char) snake_type;
      game_map[snake_pos->y_m_][++snake_pos->x_m_] = (char) snake_type;
      game_map[snake_pos->y_b_][++snake_pos->x_b_] = (char) snake_type;
    }
    break;

  case 'u':
    if(*prev_direction == 'l' || *prev_direction == 'r')
    {
      //rotate into vertical
      game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
      game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;

      game_map[--snake_pos->y_f_][++snake_pos->x_f_] = (char) snake_type;
      game_map[++snake_pos->y_b_][--snake_pos->x_b_] = (char) snake_type;
    }
    else
    {
      if(snake_pos->y_f_ < 4)
      {
        if(snake_type == USER_SNAKE)
        {
          lifepoints = 0;
          break;
        }
        else
        {
          //removing snake
          game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
          game_map[snake_pos->y_m_][snake_pos->x_m_] = (char) EMPTY;
          game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;

          //revoking on the opposite site of the map
          snake_pos->y_f_ = MAP_LENGTH - 3;
          snake_pos->y_m_ = MAP_LENGTH - 2;
          snake_pos->y_b_ = MAP_LENGTH - 1;
          game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) snake_type;
          game_map[snake_pos->y_m_][snake_pos->x_m_] = (char) snake_type;
          game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) snake_type;
          break;
        }
      }
      game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;
      game_map[--snake_pos->y_f_][snake_pos->x_f_] = (char) snake_type;
      game_map[--snake_pos->y_m_][snake_pos->x_m_] = (char) snake_type;
      game_map[--snake_pos->y_b_][snake_pos->x_b_] = (char) snake_type;
    }

    break;

  case 'd':
    if(*prev_direction == 'l' || *prev_direction == 'r')
    {
      //rotate into vertical
      game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
      game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;

      game_map[--snake_pos->y_f_][++snake_pos->x_f_] = (char) snake_type;
      game_map[++snake_pos->y_b_][--snake_pos->x_b_] = (char) snake_type;
    }
    else
    {
      if(snake_pos->y_b_ > MAP_LENGTH - 2)
      {
        if(snake_type == USER_SNAKE)
        {
          lifepoints = 0;
          break;
        }
        else
        {
          //removing snake
          game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
          game_map[snake_pos->y_m_][snake_pos->x_m_] = (char) EMPTY;
          game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) EMPTY;

          //revoking on the opposite site of the map
          snake_pos->y_f_ = 3;
          snake_pos->y_m_ = 4;
          snake_pos->y_b_ = 5;
          game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) snake_type;
          game_map[snake_pos->y_m_][snake_pos->x_m_] = (char) snake_type;
          game_map[snake_pos->y_b_][snake_pos->x_b_] = (char) snake_type;
          break;
        }
      }
      game_map[snake_pos->y_f_][snake_pos->x_f_] = (char) EMPTY;
      game_map[++snake_pos->y_f_][snake_pos->x_f_] = (char) snake_type;
      game_map[++snake_pos->y_m_][snake_pos->x_m_] = (char) snake_type;
      game_map[++snake_pos->y_b_][snake_pos->x_b_] = (char) snake_type;
    }
    break;
  default:
    break;
  }
  *prev_direction = *direction;
}
int end_game(void* rvalue, void**rvalues_enemies, void* rvalue_chest)
{
  mvwprintw(game_window, (MAP_LENGTH/2) ,16, "    GAME OVER    ");
  mvwprintw(game_window, (MAP_LENGTH/2 + 1) ,16, " %08d POINTS ",points);
  wrefresh(game_window);
  usleep(5000000);
  wborder(game_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wclear(game_window);
  wrefresh(game_window);
  delwin(game_window);
  endwin();


  assert(rvalue || true);
  assert(rvalues_enemies || true);
  assert(rvalue_chest || true);

  return 0;
}
