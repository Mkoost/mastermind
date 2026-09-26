#ifndef GAMES_H
#define GAMES_H

#include "game_api/mastermind.h"
#include "stdlib.h"
#include <stdio.h>

void game_pve(size_t, size_t, game_error_t *);
void game_evp(size_t, size_t, game_error_t *);

#endif