#ifndef CLI_H
#define CLI_H

#include "game_api/mastermind.h"

enum
{
    CLI_ZERO_ARGC = PREV_GAME_ERROR_CODE + 1,
    CLI_WRONG_ARG = PREV_GAME_ERROR_CODE + 2,
    CLI_NULLPTR_ARGV = PREV_GAME_ERROR_CODE + 3
};

enum
{
    CLI_TASK_GAME = 0,
    CLI_TASK_HELP = 1,
    CLI_TASK_LIST = 2
};

enum
{
    CLI_DIFFICULTY_BASE = 0,
    CLI_DIFFICULTY_HARD = 1,
};

enum
{
    CLI_GAMEMODE_PVE = 0,
    CLI_GAMEMODE_EVP = 1,
};

#undef PREV_GAME_ERROR_CODE
#define PREV_GAME_ERROR_CODE CLI_NULLPTR_ARGV

typedef int token_id_t;

extern char difficulty;
extern char versus_mode;
extern char task_type;

token_id_t token_encoding(const char *token);

void parse_args(int argc, char *argv[], game_error_t *e);

#endif