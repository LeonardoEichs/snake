#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
using namespace std;

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define BOARD_ROW 20
#define BOARD_COL 40
#define TIME 90000

struct snake_segment {
  int snake_pos_y;
  int snake_pos_x;
};

char board[BOARD_ROW][BOARD_COL];

vector<snake_segment> snake;
snake_segment head = {3, 5};

int fruit_pos_y;
int fruit_pos_x;

void generateFruit() {
  bool permission_position;
  do {
      fruit_pos_y = rand() % BOARD_ROW;
      fruit_pos_x = rand() % BOARD_COL;
      for (int i = 0; i < snake.size(); i++) {
        if (fruit_pos_x == snake.at(i).snake_pos_x && fruit_pos_y == snake.at(i).snake_pos_y){
          permission_position = false;
        }
        else permission_position = true;
      }
  } while (!permission_position);
  board[fruit_pos_y][fruit_pos_x] = '*';
}

void generateBoard() {
  for (int i = 0; i < BOARD_ROW; i++) {
      for (int j = 0; j < BOARD_COL; j++) {
          board[i][j] = '.';
      }
  }
}

void printBoard() {
  board[fruit_pos_y][fruit_pos_x] = '*';
  for (int i = 0; i < BOARD_ROW; i++) {
      for (int j = 0; j < BOARD_COL; j++) {
          addch(board[i][j]);
      }
      addch('\n');
  }
}

bool collisionCheck() {
  if (snake.at(0).snake_pos_x == BOARD_COL)
      return true;
  else if (snake.at(0).snake_pos_x == -1)
      return true;
  else if (snake.at(0).snake_pos_y == BOARD_ROW)
      return true;
  else if (snake.at(0).snake_pos_y == -1)
      return true;
  else
      for (int i = 1; i < snake.size(); i++) {
        if (snake.at(0).snake_pos_x == snake.at(i).snake_pos_x &&
            snake.at(0).snake_pos_y == snake.at(i).snake_pos_y)
            return true;
      }
  return false;
}

void drawSnake() {
  board[snake.at(0).snake_pos_y][snake.at(0).snake_pos_x] = 'O';
  for (int i = 1; i < snake.size(); i++){
    board[snake.at(i).snake_pos_y][snake.at(i).snake_pos_x] = 'o';
  }
}

int main() {

    int clock = TIME;

    initscr();
    timeout(0.0005);
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    curs_set(FALSE);

    srand(time(NULL));

    generateBoard();
    board[head.snake_pos_y][head.snake_pos_x] = 'O';
    snake.push_back(head);
    generateFruit();

    printBoard();

    int current_KB;
    char c;
    int points = 0;


    while(1) {
        c = getch();
        if (c == 'w') {
            if (current_KB != KB_DOWN || snake.size() == 1)
              current_KB = KB_UP;
        }
        else if (c == 's') {
            if (current_KB != KB_UP || snake.size() == 1)
              current_KB = KB_DOWN;
        }
        else if (c == 'd') {
            if (current_KB != KB_LEFT || snake.size() == 1)
              current_KB = KB_RIGHT;
        }
        else if (c == 'a') {
            if (current_KB != KB_RIGHT || snake.size() == 1)
              current_KB = KB_LEFT;

        }

        for (int i = 0; i < snake.size(); i++) {
          board[snake.at(i).snake_pos_y][snake.at(i).snake_pos_x] = '.';
        }
        switch(current_KB) {
            case KB_UP:
                clock = TIME * 1.3;
                for (int i = snake.size()-1; i >= 1; i--){
                  snake.at(i).snake_pos_y = snake.at(i-1).snake_pos_y;
                  snake.at(i).snake_pos_x = snake.at(i-1).snake_pos_x;
                }
                snake.at(0).snake_pos_y -= 1;
                break;
            case KB_DOWN:
                clock = TIME * 1.3;
                for (int i = snake.size()-1; i >= 1; i--){
                  snake.at(i).snake_pos_y = snake.at(i-1).snake_pos_y;
                  snake.at(i).snake_pos_x = snake.at(i-1).snake_pos_x;

                }
                snake.at(0).snake_pos_y += 1;
                break;
            case KB_RIGHT:
                clock = TIME;
                for (int i = snake.size()-1; i >= 1; i--){
                  snake.at(i).snake_pos_y = snake.at(i-1).snake_pos_y;
                  snake.at(i).snake_pos_x = snake.at(i-1).snake_pos_x;
                }
                snake.at(0).snake_pos_x += 1;
                break;
            case KB_LEFT:
                clock = TIME;
                for (int i = snake.size()-1; i >= 1; i--){
                  snake.at(i).snake_pos_y = snake.at(i-1).snake_pos_y;
                  snake.at(i).snake_pos_x = snake.at(i-1).snake_pos_x;
                }
                snake.at(0).snake_pos_x -= 1;
                break;
        }
        clear();
        drawSnake();
        printBoard();
        printf("Points: %d\n", points);
        if (snake.at(0).snake_pos_x == fruit_pos_x && snake.at(0).snake_pos_y == fruit_pos_y) {
          points += 10;
          switch(current_KB) {
              case KB_UP:
                  snake.push_back({snake.at((int) snake.size()-1).snake_pos_y+1, snake.at((int) snake.size()-1).snake_pos_x});
                  break;
              case KB_DOWN:
                  snake.push_back({snake.at((int) snake.size()-1).snake_pos_y-1, snake.at((int) snake.size()-1).snake_pos_x});
                  break;
              case KB_RIGHT:
                  snake.push_back({snake.at((int) snake.size()-1).snake_pos_y, snake.at((int) snake.size()-1).snake_pos_x-1});
                  break;
              case KB_LEFT:
                  snake.push_back({snake.at((int) snake.size()-1).snake_pos_y, snake.at((int) snake.size()-1).snake_pos_x+1});
                  break;
          }
          generateFruit();
        }
        if (collisionCheck())
          break;
        usleep(clock);
    }
    return 0;
}
