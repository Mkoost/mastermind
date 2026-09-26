#include "games.h"
#include "game_solver.h"

// TODO: Доделать обработку ошибок
// TODO: Доделать запись в файл
void game_pve(size_t size, size_t abc, game_error_t *e)
{
    game_error_t err = GAME_SUCCESS;
    game_init(size, abc, &err);

    if (err != GAME_SUCCESS)
    {
        if (e)
            *e = err;
        return;
    }

    char *buffer = malloc(size + 1);
    if (!buffer)
    {
        err = GAME_MALLOC_NULL_PTR;
        if (e)
            *e = err;
        goto Finalize;
    }

    char flag = 1;
    while (flag)
    {
        size_t i = 0;
        int c = 0;

        while (i < size && (c = getchar()) != EOF && c != '\n')
            buffer[i++] = (char)c;
        buffer[i] = '\0';

        while (c != '\n' && c != EOF)
            c = getchar();

        if (c == EOF && i == 0)
        {
            printf("\nEOF. Exiting.\n");
            err = GAME_WRONG_ARG_FORMAT;
            goto Error;
        }

        CowNBulls answer = make_attempt(buffer, &err);

        if (err == GAME_WRONG_ARG_FORMAT)
        {
            printf("Wrong format. Try again!\n");
            continue;
        }
        if (err != GAME_SUCCESS)
        {
            if (e)
                *e = err;
            goto Finalize;
        }

        printf("Cows: %zu\nBulls: %zu\n", answer.cows, answer.bulls);
        flag = (answer.bulls != size);
    }

    free(buffer);
    buffer = NULL;

    printf("You win!\n"
           "Write name (less than %d characters):\n",
           SCORELIST_NAME_SIZE);

    ScoreNode node = { .name = {0}, .attempts = get_attempts(),
                       .next = NULL, .prev = NULL };
    int i = 0;
    int c = 0;

    while (i < SCORELIST_NAME_SIZE - 1 && (c = getchar()) != EOF && c != '\n')
        node.name[i++] = (char)c;
    node.name[i] = '\0';

    while (c != '\n' && c != EOF)
        c = getchar();


    goto Finalize;

Finalize:
    free(buffer);     
    game_finalize();
    return;

Error:
    ERROR_HANDLER___(err, "Critical input error", e);
    free(buffer);
    game_finalize();
    return;
}

// TODO: Доделать обработку ошибок
// TODO: Доделать запись в файл
void game_evp(size_t size, size_t abc, game_error_t *e)
{
    game_error_t err = GAME_SUCCESS;
    game_init(size, abc, &err);
    if (err != GAME_SUCCESS)
    {
        if (e)
            *e = err;
        return;
    }

    printf("Enter the secret number (%zu digits, each in [1; %zu]):\n",
           size, abc);

    char *secret = malloc(size + 1);
    if (!secret)
    {
        err = GAME_MALLOC_NULL_PTR;
        if (e)
            *e = err;
        goto Finalize;
    }

    size_t i = 0;
    int c = 0;

    while (i < size && (c = getchar()) != EOF && c != '\n')
        secret[i++] = (char)c;
    secret[i] = '\0';

    while (c != '\n' && c != EOF)
        c = getchar();

    set_number(secret, &err);
    free(secret);

    if (err != GAME_SUCCESS)
    {
        if (e)
            *e = err;
        goto Finalize;
    }

    CodeSet set;
    solver_init(&set, size, abc);
    if (!set.all_codes)
    {
        err = GAME_MALLOC_NULL_PTR;
        if (e)
            *e = err;
        goto Finalize;
    }

    int solved = 0;
    while (!solved)
    {
        const char *guess = solver_next_guess(&set);

        printf("Guess #%zu: %.*s\n",
               get_attempts() + 1, (int)size, guess);

        CowNBulls response = make_attempt(guess, &err);
        if (err != GAME_SUCCESS)
        {
            if (e)
                *e = err;
            break;
        }

        printf("  Bulls: %zu, Cows: %zu\n",
               response.bulls, response.cows);

        if (response.bulls == size)
        {
            printf("Solved in %zu attempts!\n", get_attempts());
            solved = 1;
            break;
        }

        solver_filter(&set, guess, response);

        if (solver_count(&set) == 0)
        {
            printf("Inconsistent responses - no candidates left.\n");
            err = GAME_WRONG_ARG_FORMAT;
            if (e)
                *e = err;
            break;
        }
    }

    solver_finalize(&set);

Finalize:
    game_finalize();
    return;

Error:
    ERROR_HANDLER___(err, "Input error", e);
    game_finalize();
    return;
}