#ifndef SCORELIST_H
#define SCORELIST_H

#include "stdlib.h"
#include "string.h"
#include "game_error_api.h"

#define SCORELIST_NAME_SIZE 16

enum
{
    SCORELIST_EMPTY = PREV_GAME_ERROR_CODE + 1,
    SCORELIST_INVALID_INDEX = PREV_GAME_ERROR_CODE + 2
};

#undef PREV_GAME_ERROR_CODE
#define PREV_GAME_ERROR_CODE SCORELIST_INVALID_INDEX

typedef struct ScoreNode
{
    char name[SCORELIST_NAME_SIZE];
    unsigned attempts;
    struct ScoreNode *next;
    struct ScoreNode *prev;
} ScoreNode;

typedef struct
{
    ScoreNode *head;
    size_t size;
} ScoreList;

void ScoreList_init(ScoreList *list);
void ScoreList_free(ScoreList *list);

void ScoreList_push_back(ScoreList *list, const char *name, unsigned attempts, game_error_t *e);
void ScoreList_push_forward(ScoreList *list, const char *name, unsigned attempts, game_error_t *e);
void ScoreList_insert(ScoreList *list, const char *name, unsigned attempts, size_t index, game_error_t *e);

ScoreNode ScoreList_pop_back(ScoreList *list, game_error_t *e);
ScoreNode ScoreList_pop_forward(ScoreList *list, game_error_t *e);

ScoreNode *ScoreList_get_node(ScoreList *list, size_t index);

#endif