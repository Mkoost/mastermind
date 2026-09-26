#ifndef GAME_SOLVER_H
#define GAME_SOLVER_H

#include <stddef.h>
#include "game_api.h"

// TODO: Сделать обработку ошибок

typedef struct
{
    char *all_codes;
    size_t all_count;
    size_t *candidates;
    char *is_candidate;
    size_t count;
    size_t n;
    size_t k;
    char *guess_buf;
} CodeSet;

void solver_init(CodeSet *set, size_t n, size_t k);

void solver_finalize(CodeSet *set);

void solver_filter(CodeSet *set, const char *guess, CowNBulls response);

const char *solver_next_guess(CodeSet *set);

size_t solver_count(const CodeSet *set);

#endif