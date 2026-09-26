#include "game_api.h"

#define MAX_DIGIT_NUMBER 10

static size_t attempts_number = 0;
static size_t number_size = 0;
static size_t digit_number = 0;
static char inited = 0;

static char *guessed_number = NULL;

size_t get_attempts() { return attempts_number; }
size_t get_game_size() { return number_size; }
size_t get_digit_number() { return digit_number; }
char is_inited() { return inited; }

void game_init(size_t num_sz, size_t d_num, game_error_t *e)
{
    if (!num_sz)
        goto InvalidArg;

    if (d_num >= MAX_DIGIT_NUMBER || d_num == 0)
        goto InvalidArg;

    if (guessed_number)
        goto DoubleInit;

    attempts_number = 0;
    digit_number = d_num;
    number_size = num_sz;

    guessed_number = malloc(num_sz + 1);
    if (!guessed_number)
        goto MallocNullPtr;

    srand(time(NULL));

    for (size_t i = 0; i < number_size; ++i)
    {
        char rnd_digit = 1 + rand() % digit_number;
        guessed_number[i] = rnd_digit + '0';
    }
    guessed_number[number_size] = '\0';

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    inited = 1;
    return;

InvalidArg:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT,
                     "Argument num_sz must be greater than 0, d_num must be in [1; 9]", e);
    game_finalize();
    return;

DoubleInit:
    ERROR_HANDLER___(GAME_DOUBLE_INIT,
                     "Global game variables already initialized", e);
    return;

MallocNullPtr:
    ERROR_HANDLER___(GAME_MALLOC_NULL_PTR, "Cannot allocate data", e);
    game_finalize();
    return;
}

void game_finalize()
{
    free(guessed_number);
    guessed_number = NULL;

    number_size = 0;
    digit_number = 0;
    attempts_number = 0;
    inited = 0;
}


CowNBulls count_bulls_cows(const char *secret, const char *guess, size_t n)
{
    CowNBulls result = {0, 0};
    unsigned count_secret[MAX_DIGIT_NUMBER] = {0};
    unsigned count_guess[MAX_DIGIT_NUMBER] = {0};

    for (size_t i = 0; i < n; ++i)
    {
        if (secret[i] == guess[i])
        {
            result.bulls++;
        }
        else
        {
            count_secret[secret[i] - '0']++;
            count_guess[guess[i] - '0']++;
        }
    }

    for (int d = 0; d < MAX_DIGIT_NUMBER; ++d)
    {
        result.cows += (count_secret[d] < count_guess[d])
                           ? count_secret[d]
                           : count_guess[d];
    }

    return result;
}

/* ===== Основная операция ===== */

CowNBulls make_attempt(char const *number, game_error_t *e)
{
    CowNBulls result = {0, 0};

    if (!inited)
        goto GameNotInit;

    if (!number || strlen(number) != number_size)
        goto InvalidArg;

    for (size_t i = 0; i < number_size; ++i)
        if (number[i] < '1' || number[i] > '0' + digit_number)
            goto InvalidArg;

    result = count_bulls_cows(guessed_number, number, number_size);
    attempts_number++;

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    return result;

GameNotInit:
    ERROR_HANDLER___(GAME_UNINITED, "Game not initialized", e);
    return (CowNBulls){0, 0};

InvalidArg:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "Invalid number string", e);
    return (CowNBulls){0, 0};
}

/* ===== Управление секретом ===== */

char *get_number()
{
    return guessed_number;
}

void set_number(const char *number, game_error_t *e)
{
    if (!inited)
        goto GameNotInit;

    if (!number)
        goto InvalidArg;

    if (strlen(number) != number_size)
        goto InvalidArg;

    for (size_t i = 0; i < number_size; ++i)
        if (number[i] < '1' || number[i] > '0' + digit_number)
            goto InvalidArg;

    strcpy(guessed_number, number);

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    return;

GameNotInit:
    ERROR_HANDLER___(GAME_UNINITED, "Game not initialized", e);
    return;

InvalidArg:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "Invalid number string", e);
    return;
}

void reset_attempts()
{
    attempts_number = 0;
}