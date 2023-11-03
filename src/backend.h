#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// #define SIZE 4
#define EMPTY -1
#define PRESENT 1
#define TEXT_MODE 0
#define IMAGE_MODE 1

// Game data
#define GAME_SIZE 4
#define GAME_TEXT "SAPOATORCASAUVA"

struct Game {
  short* positions;
  int size;
  char* text;
  short empty;
};

typedef struct Game Game;

Game game;

void shuffle(short *array, size_t n) {
  if (n > 1) {
    for (size_t i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

void log_game_data() {
  printf("Game size: %d\n", game.size);
  printf("Game text: %s\n", game.text);
}

void init_game() {
  game.size = GAME_SIZE;


  game.text = GAME_TEXT;

  log_game_data();
  short *shuffled = malloc(sizeof(short) * ((game.size * game.size) - 1));
  for (int i = 0; i < (game.size * game.size) - 1; i++) {
    shuffled[i] = i;
  }
  shuffle(shuffled, (game.size * game.size) - 1);
  game.positions = malloc(sizeof(short) * game.size * game.size);
  for (int i = 0; i < (game.size * game.size) - 1; i++) {
    game.positions[i] = shuffled[i];
  }
  game.positions[game.size * game.size - 1] = EMPTY;
  game.empty = game.size * game.size - 1;
}

short check_finished() {
  if (game.positions[game.size * game.size - 1] != EMPTY) {
    return 0;
  }
  for (int i = 0; i < game.size * game.size - 1; i++) {
    if (game.text[game.positions[i]] != game.text[i]) {
      return 0;
    }
  }
  return 1;
}

void move_up() {
  if (game.empty + game.size < game.size * game.size) {
    game.positions[game.empty] = game.positions[game.empty + game.size];
    game.positions[game.empty + game.size] = EMPTY;
    game.empty += game.size;
  }
}

void move_down() {
  if (game.empty - game.size >= 0) {
    game.positions[game.empty] = game.positions[game.empty - game.size];
    game.positions[game.empty - game.size] = EMPTY;
    game.empty -= game.size;
  }
}

void move_left() {
  short line = game.empty / game.size;
  short next_line = (game.empty + 1) / game.size;
  if (line == next_line && game.empty + 1 < game.size * game.size) {
    game.positions[game.empty] = game.positions[game.empty + 1];
    game.positions[game.empty + 1] = EMPTY;
    game.empty += 1;
  }
}

void move_right() {
  short line = game.empty / game.size;
  short next_line = (game.empty - 1) / game.size;
  if (line == next_line && game.empty - 1 >= 0) {
    game.positions[game.empty] = game.positions[game.empty - 1];
    game.positions[game.empty - 1] = EMPTY;
    game.empty -= 1;
  }
}
