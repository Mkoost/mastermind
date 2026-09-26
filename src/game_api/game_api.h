#ifndef GAME_API_H
#define GAME_API_H

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

#include "game_error_api.h"

typedef struct
{
    size_t bulls;
    size_t cows;
} CowNBulls;

void game_init(size_t, size_t, game_error_t *);
void game_finalize();

size_t get_attempts();
size_t get_game_size();
size_t get_digit_number();
char is_inited();

void set_number(const char *, game_error_t *);
char *get_number();

void reset_attempts();
CowNBulls make_attempt(char const *, game_error_t *);

CowNBulls count_bulls_cows(const char *, const char *, size_t);

#endif