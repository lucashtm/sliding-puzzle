#ifndef BACKEND_H
#define BACKEND_H

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

extern Game game;

void shuffle(short *array, size_t n);

void log_game_data();

void init_game();

short check_finished();

void move_up();
void move_down();
void move_left();
void move_right();
#endif
