#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

int main() {
  int X = 0;
  int O = 1;
  int grid[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

  int playersTurn = X; // 1 is O, 0 is X
  int quit = 0;

  char field[2];
  printf("Welcome to ConsoleTicTacToe\n");
  printf("You can leave the game with q at any point\n");

  while(quit != 1) {
    printf("╔═══╦═══╦═══╗	\n");
    printf("║ %c ║", grid[0] == -1 ? '-' : grid[0] == X ? 'X' : 'O');
    printf(" %c ║", grid[1] == -1 ? '-' : grid[1] == X ? 'X' : 'O');
    printf(" %c ║\n", grid[2] == -1 ? '-' : grid[2] == X ? 'X' : 'O');
    printf("╠═══╬═══╬═══╣\n");
    printf("║ %c ║", grid[3] == -1 ? '-' : grid[3] == X ? 'X' : 'O');
    printf(" %c ║", grid[4] == -1 ? '-' : grid[4] == X ? 'X' : 'O');
    printf(" %c ║\n", grid[5] == -1 ? '-' : grid[5] == X ? 'X' : 'O');
    printf("╠═══╬═══╬═══╣\n");
    printf("║ %c ║", grid[6] == -1 ? '-' : grid[6] == X ? 'X' : 'O');
    printf(" %c ║", grid[7] == -1 ? '-' : grid[7] == X ? 'X' : 'O');
    printf(" %c ║\n", grid[8] == -1 ? '-' : grid[8] == X ? 'X' : 'O');
    printf("╚═══╩═══╩═══╝\n");

    if (!playersTurn) {
      printf("Please enter field value for X (0-9):\n");
    } else {
      printf("Please enter field value for O (0-9):\n");
    }
    scanf("%s", field);
    if (field[0] == 'q') {
      quit = 1;
      continue;
    } else {
      int f = field[0] - 48;
      if (grid[f] > -1) {
        printf("This field has already been played pick another one (0-9):\n");
        continue;
      } else {
        grid[f] = playersTurn;
        playersTurn = playersTurn == 0 ? O : X;
      }

      if (
          (grid[0] == X && grid[1] == X && grid[2] == X) || // Horizontal
          (grid[3] == X && grid[4] == X && grid[5] == X) || // Horizontal
          (grid[6] == X && grid[7] == X && grid[8] == X) || // Horizontal
          (grid[0] == X && grid[4] == X && grid[8] == X) || // Diagonal
          (grid[6] == X && grid[4] == X && grid[2] == X) || // Diagonal
          (grid[0] == X && grid[3] == X && grid[6] == X) || // Vertical
          (grid[1] == X && grid[4] == X && grid[7] == X) || // Vertical
          (grid[2] == X && grid[5] == X && grid[8] == X)    // Vertical
          ) {
        printf("Player X won");
        break;
      }

      if (
          (grid[0] == O && grid[1] == O && grid[2] == O) || // Horizontal
          (grid[3] == O && grid[4] == O && grid[5] == O) || // Horizontal
          (grid[6] == O && grid[7] == O && grid[8] == O) || // Horizontal
          (grid[0] == O && grid[4] == O && grid[8] == O) || // Diagonal
          (grid[6] == O && grid[4] == O && grid[2] == O) || // Diagonal
          (grid[0] == O && grid[3] == O && grid[6] == O) || // Vertical
          (grid[1] == O && grid[4] == O && grid[7] == O) || // Vertical
          (grid[2] == O && grid[5] == O && grid[8] == O)    // Vertical
          ) {
        printf("Player O won");
        break;
      }
    }
  }
  return 0;
}
