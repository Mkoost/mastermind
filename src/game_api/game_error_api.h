#ifndef GAME_ERROR_API_H
#define GAME_ERROR_API_H

#include "string.h"

enum
{
    GAME_SUCCESS = 0,
    GAME_UNINITED = 1,
    GAME_MALLOC_NULL_PTR = 2,
    GAME_WRONG_ARG_FORMAT = 3,
    GAME_DOUBLE_INIT = 4,
};

#define PREV_GAME_ERROR_CODE GAME_DOUBLE_INIT

#define MAX_ERROR_MESSAGE_SIZE 500

typedef unsigned game_error_t;

extern char error_message[MAX_ERROR_MESSAGE_SIZE];
extern game_error_t last_error;

game_error_t get_last_error();
char *get_error_message();

void put_game_error(game_error_t errid, const char *msg);

#define ERROR_HANDLER___(errid, msg, e) \
    do                                  \
    {                                   \
        if (e)                          \
            *e = errid;                 \
        put_game_error(errid, msg);     \
    } while (0)

#endif