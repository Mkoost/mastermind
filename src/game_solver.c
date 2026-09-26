#include "game_solver.h"
#include <stdlib.h>
#include <string.h>

#define MAX_BUCKETS 64

void solver_init(CodeSet *set, size_t n, size_t k)
{
    set->n = n;
    set->k = k;
    set->all_codes = NULL;
    set->candidates = NULL;
    set->is_candidate = NULL;
    set->guess_buf = NULL;
    set->all_count = 0;
    set->count = 0;

    size_t total = 1;
    for (size_t i = 0; i < n; ++i)
        total *= k;

    set->all_codes = malloc(total * n);
    set->candidates = malloc(total * sizeof(size_t));
    set->is_candidate = malloc(total);
    set->guess_buf = malloc(n + 1);

    if (!set->all_codes || !set->candidates ||
        !set->is_candidate || !set->guess_buf)
    {
        solver_finalize(set);
        return;
    }

    set->all_count = total;

    for (size_t idx = 0; idx < total; ++idx)
    {
        size_t rem = idx;
        for (size_t i = 0; i < n; ++i)
        {
            set->all_codes[idx * n + i] = (char)('1' + (rem % k));
            rem /= k;
        }
    }

    for (size_t i = 0; i < total; ++i)
    {
        set->candidates[i] = i;
        set->is_candidate[i] = 1;
    }
    set->count = total;

    set->guess_buf[n] = '\0';
}

void solver_finalize(CodeSet *set)
{
    if (!set)
        return;
    free(set->all_codes);
    free(set->candidates);
    free(set->is_candidate);
    free(set->guess_buf);
    set->all_codes = NULL;
    set->candidates = NULL;
    set->is_candidate = NULL;
    set->guess_buf = NULL;
    set->all_count = 0;
    set->count = 0;
}

size_t solver_count(const CodeSet *set)
{
    return set->count;
}

void solver_filter(CodeSet *set, const char *guess, CowNBulls response)
{
    size_t write = 0;

    memset(set->is_candidate, 0, set->all_count);

    for (size_t i = 0; i < set->count; ++i)
    {
        size_t idx = set->candidates[i];
        const char *c = set->all_codes + idx * set->n;

        CowNBulls r = count_bulls_cows(c, guess, set->n);
        if (r.bulls == response.bulls && r.cows == response.cows)
        {
            set->candidates[write++] = idx;
            set->is_candidate[idx] = 1;
        }
    }
    set->count = write;
}

const char *solver_next_guess(CodeSet *set)
{
    if (set->count == 1)
    {
        memcpy(set->guess_buf,
               set->all_codes + set->candidates[0] * set->n,
               set->n);
        return set->guess_buf;
    }

    size_t best_worst = (size_t)-1;
    size_t best_idx = 0;
    int best_is_cand = 0;

    for (size_t g = 0; g < set->all_count; ++g)
    {
        const char *guess = set->all_codes + g * set->n;

        size_t buckets[MAX_BUCKETS] = {0};
        size_t worst = 0;

        for (size_t i = 0; i < set->count; ++i)
        {
            const char *c = set->all_codes + set->candidates[i] * set->n;
            CowNBulls r = count_bulls_cows(c, guess, set->n);
            size_t bidx = r.bulls * (set->n + 1) + r.cows;

            buckets[bidx]++;
            if (buckets[bidx] > worst)
                worst = buckets[bidx];
        }

        if (worst > best_worst)
            continue;

        int is_cand = set->is_candidate[g];

        if (worst < best_worst ||
            (worst == best_worst && is_cand && !best_is_cand))
        {
            best_worst = worst;
            best_idx = g;
            best_is_cand = is_cand;
        }
    }

    memcpy(set->guess_buf, set->all_codes + best_idx * set->n, set->n);
    return set->guess_buf;
}