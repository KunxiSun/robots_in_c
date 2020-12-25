#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN (100)
#define L (8)

struct Human {
  int x;
  int y;
  int pre_x;
  int pre_y;
  char short_name;
};
struct Exit {
  int x;
  int y;
  char short_name;
};
struct Robots {
  int num;
  int x[30];
  int y[30];
  char short_name;
  int statu[30];
};

struct Human human = {-1, -1, -1, -1, 'H'};
struct Exit exi = {-1, -1, 'E'};
struct Robots robots = {0, {-1}, {-1}, 'R', {-1}};

void print_board(char board[L][L]) {
  puts("");
  for (int i = 0; i < L; i++) {
    printf("|");
    for (int j = 0; j < L; j++) {
      printf("%c|", board[i][j]);  // print y coordinate
    }
    puts("");  // print the next line
  }
  puts("");
}

int check_coordinate(int x, int y) {
  return x >= 0 && x <= 7 && y >= 0 && y <= 7;
}

int check_overlap(int robX, int robY){
  int overlap = 0;
  for (int i = 0; i < robots.num; i++) {
    if (robX == robots.x[i] && robY == robots.y[i]) {
      overlap = 1;
      break;
    }
  }
  return overlap;
}

int check_distance(int humanX, int humanY, int robX, int robY){
  int x = humanX-robX;
  int y = humanY-robY;
  return (abs(x)>=2 || abs(y)>=2);
}

void configuration(char board[L][L]) {
  int humanX, humanY, numR, robX, robY;
  char input[10];

  // place human
  while (1) {
    printf("%s: ", "Human starting point");
    fgets(input, 10, stdin);
    int match = sscanf(input, "%d %d", &humanX, &humanY);
    if (check_coordinate(humanX, humanY) && match) {
      human.x = humanX;
      human.y = humanY;
      human.pre_x = humanX;
      human.pre_y = humanY;
      board[humanY][humanX] = human.short_name;
      break;
    } else {
      puts("Invalid Coordinates, Try again");
    }
  }

  // place robot
  while (1) {
    printf("%s: ", "Number of robots");
    fgets(input, 10, stdin);
    int match = sscanf(input, "%d", &numR);
    if (numR > 0 && numR <= 30 && match) {
      robots.num = numR;
      break;
    } else {
      puts("Invalid number, Try again");
    }
  }
  for (int i = 0; i < numR; i++) {
    while (1) {
      printf("Robot %d: ", i + 1);
      fgets(input, 10, stdin);
      int match = sscanf(input, "%d %d", &robX, &robY);
      if (check_coordinate(robX, robY) && match && check_distance(humanX, humanY, robX, robY)
            && !check_overlap(robX, robY)) {
          robots.x[i] = robX;
          robots.y[i] = robY;
          robots.statu[i] = 1;
          board[robY][robX] = robots.short_name;
          break;
      } else {
        puts("Invalid Coordinates, Try again");
      }
    }
  }

  // place exit
  
  int exitX = rand() % L;
  int exitY = rand() % L;
  exi.x = exitX;
  exi.y = exitY;
  board[exitY][exitX] = exi.short_name;

  puts("");
}

void human_move(char board[L][L], char direction[10]) {
  board[human.y][human.x] = ' ';
  human.pre_x = human.x;
  human.pre_y = human.y;
  if (strcmp(direction, "NT") == 0) {
    board[human.y - 1][human.x] = human.short_name;
    human.y -= 1;
  } else if (strcmp(direction, "ST") == 0) {
    board[human.y + 1][human.x] = human.short_name;
    human.y += 1;
  } else if (strcmp(direction, "WT") == 0) {
    board[human.y][human.x - 1] = human.short_name;
    human.x -= 1;
  } else if (strcmp(direction, "ET") == 0) {
    board[human.y][human.x + 1] = human.short_name;
    human.x += 1;
  }
}

void robots_move(char board[L][L]) {
  for (int i = 0; i < robots.num; i++) {
    if (robots.statu[i] == 1) {
      int x_distance = human.x - robots.x[i];
      int y_distance = human.y - robots.y[i];
      board[robots.y[i]][robots.x[i]] = ' ';
      if (abs(x_distance) < abs(y_distance)) {
        if (y_distance > 0) {  // human is at bottom of robot
          board[robots.y[i] + 1][robots.x[i]] = robots.short_name;
          robots.y[i] += 1;
        } else if (y_distance < 0) {  // human is at top of robot
          board[robots.y[i] - 1][robots.x[i]] = robots.short_name;
          robots.y[i] -= 1;
        }
      } else if (abs(x_distance) > abs(y_distance)) {
        if (x_distance > 0) {  // human is at right of robot
          board[robots.y[i]][robots.x[i] + 1] = robots.short_name;
          robots.x[i] += 1;
        } else if (x_distance < 0) {  // human is at left of robot
          board[robots.y[i]][robots.x[i] - 1] = robots.short_name;
          robots.x[i] -= 1;
        }
      } else {
        // human move on y-axis, robot move on x-axis
        if (human.x == human.pre_x) {
          if (x_distance > 0) {
            board[robots.y[i]][robots.x[i] + 1] = robots.short_name;
            robots.x[i] += 1;
          } else if (x_distance < 0) {
            board[robots.y[i]][robots.x[i] - 1] = robots.short_name;
            robots.x[i] -= 1;
          }
        }
        // human move on x-axis, robot move on y-axis
        if (human.y == human.pre_y) {
          if (y_distance > 0) {
            board[robots.y[i] + 1][robots.x[i]] = robots.short_name;
            robots.y[i] += 1;
          } else if (y_distance < 0) {
            board[robots.y[i] + 1][robots.x[i]] = robots.short_name;
            robots.y[i] -= 1;
          }
        }
      }
    }
  }
}

void check_robot_collision(char board[L][L]) {
  for (int i = 0; i < robots.num; i++) {
    for (int j = 0; j < robots.num; j++) {
      if (i != j && robots.x[i] == robots.x[j] && robots.y[i] == robots.y[j]) {
        robots.statu[i] = 0;
        robots.statu[j] = 0;
        board[robots.y[i]][robots.x[j]] = 'D';
      }
    }
  }
}

int check_win() {
  int allDestoryed = 1;
  for (int i = 0; i < robots.num; i++) {
    if (robots.statu[i] == 1) {
      allDestoryed = 0;
      break;
    }
  }

  int reachExit = 0;
  if (exi.x == human.x && exi.y == human.y) {
    reachExit = 1;
  }

  return reachExit == 1 || allDestoryed == 1;
}

int check_doomed(){
  int doomed = 0;
  for (int i = 0; i < robots.num; i++) {
    if (robots.statu[i] == 1 && human.x == robots.x[i] && human.y == robots.y[i]) {
      doomed = 1;
      break;
    }
  }
  return doomed;
}

int main(int argc, char** argv) {

  srand(atoi(argv[1]));

  while (1) {
    char buf[BUF_LEN];
    char board[L][L];
    char command[10];
    char direction[10];

    memset(board, ' ', L * L);
    
    configuration(board);

    while (fgets(buf, BUF_LEN, stdin)) {

      check_robot_collision(board);
      sscanf(buf, "%s %s", command, direction);

      if (strcmp(command, "exit") == 0) {
        return 0;
      } else if (strcmp(command, "display") == 0) {
        print_board(board);
      } else if (strcmp(command, "move") == 0) {
        human_move(board, direction);
        robots_move(board);
        puts("");
      }

      if (check_win() == 1) {
        puts("Well done! You survived, but it isn't over!\n\n");
        break;
      }

      if(check_doomed()){
        puts("The robots got you! We are doomed!\n");
        break;
      }
    }
  }
}