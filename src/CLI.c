#include "CLI.h"
#include <errno.h>

char difficulty = CLI_DIFFICULTY_BASE;
char versus_mode = CLI_GAMEMODE_PVE;
char task_type = CLI_TASK_GAME;

static enum
{
    PASS_CODE = 0,
    H_CODE = 1,
    M_CODE = 2,
    D_CODE = 3,
    L_CODE = 4,
    ERROR_CODE = -1
};

static enum
{
    DEFAULT_MODE = 10,
    HELP_MODE = 11,
    LIST_MODE = 12,
    GAME_CHANGE_MODE = 13,
    DIFFICULTY_MODE = 14,
    ERROR_MODE = -1
};

token_id_t token_encoding(const char *token)
{
    if (!token || token[0] != '-')
        return PASS_CODE;

    switch (token[1])
    {
    case 'h':
        return H_CODE;
    case 'm':
        return M_CODE;
    case 'd':
        return D_CODE;
    case 'l':
        return L_CODE;
    default:
        return ERROR_CODE;
    }
}

long parse_digit(const char *s, char *ok)
{
    char *end;
    errno = 0;
    long v = strtol(s, &end, 10);
    *ok = (end != s) && (*end == '\0') && (errno != ERANGE);
    return v;
}

void parse_args(int argc, char *argv[], game_error_t *e)
{

    if (argc == 0)
        goto ZeroArgc;
    if (argv == NULL)
        goto NullptrArgv;

    int state = DEFAULT_MODE;

    for (int i = 1; i < argc; ++i)
    {
        token_id_t id = token_encoding(argv[i]);

        switch (state)
        {
        case DEFAULT_MODE:
            switch (id)
            {
            case M_CODE:
                state = GAME_CHANGE_MODE;
                break;
            case D_CODE:
                state = DIFFICULTY_MODE;
                break;
            case L_CODE:
                task_type = CLI_TASK_LIST;
                state = LIST_MODE;
                break;
            case H_CODE:
                task_type = CLI_TASK_HELP;
                state = HELP_MODE;
                break;
            default:
                goto CLIWrongArg;
            }
            break;

        case HELP_MODE:
        case LIST_MODE:
            goto CLIWrongArg;

        case GAME_CHANGE_MODE:
        {
            if (id != PASS_CODE)
                goto CLIWrongArg;

            char ok = 0;
            long num = parse_digit(argv[i], &ok);
            if (!ok)
                goto CLIWrongArg;

            if (num == 0)
                versus_mode = CLI_GAMEMODE_PVE;
            else if (num == 1)
                versus_mode = CLI_GAMEMODE_EVP;
            else
                goto CLIWrongArg;

            state = DEFAULT_MODE;
            break;
        }

        case DIFFICULTY_MODE:
        {
            if (id != PASS_CODE)
                goto CLIWrongArg;

            char ok = 0;
            long num = parse_digit(argv[i], &ok);
            if (!ok)
                goto CLIWrongArg;

            if (num == 0)
                difficulty = CLI_DIFFICULTY_BASE;
            else if (num == 1)
                difficulty = CLI_DIFFICULTY_HARD;
            else
                goto CLIWrongArg;

            state = DEFAULT_MODE;
            break;
        }

        default:
            goto CLIWrongArg;
        }
    }

    if (state == GAME_CHANGE_MODE || state == DIFFICULTY_MODE)
        goto CLIWrongArg;

    goto Success;

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    return;

CLIWrongArg:
    ERROR_HANDLER___(CLI_WRONG_ARG, "Wrong input argument", e);
    return;

NullptrArgv:
    ERROR_HANDLER___(CLI_NULLPTR_ARGV, "Nullptr argv argument", e);
    return;

ZeroArgc:
    ERROR_HANDLER___(CLI_ZERO_ARGC, "Zero argc argument", e);
    return;
}