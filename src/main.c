#include "string.h"
#include "CLI.h"
#include "games.h"

void pass() { printf("TODO!!!\n"); }

void print_help(void);
void start_game(game_error_t *);
void print_scoreboard(game_error_t *);

/*
Интерфейс
-- PvE
-- EvP
-- Поменять сложность
-- Таблица рекордов
*/

int main(int argc, char *argv[])
{
    game_error_t err = GAME_SUCCESS;
    parse_args(argc, argv, &err);
    if (err != GAME_SUCCESS)
    {
        fprintf(stderr, "%s\n", get_error_message());
        goto Error;
    }

    switch (task_type)
    {
    case CLI_TASK_GAME:
        start_game(&err);
        break;
    case CLI_TASK_HELP:
        print_help();
        break;
    case CLI_TASK_LIST:
        pass(); // TODO: Сделать вывод ввод
        break;
    default:
        goto Error;
    }

Success:
    return 0;
Error:
    return -1;
}

void print_help(void)
{
    printf(
        "Mastermind - Bulls and Cows game\n"
        "\n"
        "Usage:\n"
        "  mastermind [options]\n"
        "\n"
        "Options:\n"
        "  -h              Show this help and exit\n"
        "  -m <0|1>        Game mode:\n"
        "                    0 - PvE (player vs computer)\n"
        "                    1 - EvP (computer vs player)\n"
        "  -d <0|1>        Difficulty level:\n"
        "                    0 - basic\n"
        "                    1 - hard\n"
        "  -l              Show scoreboard and exit\n"
        "\n"
        "Examples:\n"
        "  mastermind -m 0 -d 1     PvE, hard difficulty\n"
        "  mastermind -m 1          EvP, basic difficulty\n"
        "  mastermind -l            Scoreboard\n"
        "\n"
        "Without arguments, the game starts in default mode:\n"
        "  PvE, basic difficulty.\n");
}

void start_game(game_error_t *e)
{
    size_t game_size = 0;
    size_t game_abc = 0;

    if (difficulty == CLI_DIFFICULTY_BASE)
    {
        game_size = 4;
        game_abc = 6;
    }
    else if (difficulty == CLI_DIFFICULTY_HARD)
    {
        game_size = 5;
        game_abc = 8;
    }

    if (versus_mode == CLI_GAMEMODE_PVE)
    {
        game_pve(game_size, game_abc, e);
    }
    else if (versus_mode == CLI_GAMEMODE_EVP)
    {
        game_evp(game_size, game_abc, e);
    }
}