#include "game_error_api.h"

game_error_t get_last_error() { return last_error; }
char *get_error_message() { return error_message; }

char error_message[MAX_ERROR_MESSAGE_SIZE];
game_error_t last_error;

void put_game_error(game_error_t errid, const char *msg)
{
    last_error = errid;
    strcpy(error_message, msg);
}
