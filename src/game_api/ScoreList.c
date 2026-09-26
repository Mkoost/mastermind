#include "ScoreList.h"

static ScoreNode *create_node(const char *name, unsigned attempts, game_error_t *e)
{
    ScoreNode *node = malloc(sizeof(ScoreNode));
    if (!node)
        goto MallocNullPtr;

    strncpy(node->name, name, SCORELIST_NAME_SIZE - 1);
    node->name[SCORELIST_NAME_SIZE - 1] = '\0';
    node->attempts = attempts;
    node->next = node->prev = node;

Success:
    return node;
MallocNullPtr:
    ERROR_HANDLER___(GAME_MALLOC_NULL_PTR, "Failed to allocate ScoreNode", e);
    return NULL;
}

static ScoreNode *get_node_at(ScoreList *list, size_t index)
{
    if (!list->head || index >= list->size)
        return NULL;
    ScoreNode *cur = list->head;
    for (size_t i = 0; i < index; ++i)
    {
        cur = cur->next;
    }
    return cur;
}

void ScoreList_init(ScoreList *list)
{
    if (list)
    {
        list->head = NULL;
        list->size = 0;
    }
}

void ScoreList_free(ScoreList *list)
{
    if (!list)
        return;

    if (list->head)
    {
        ScoreNode *cur = list->head;
        ScoreNode *next;
        do
        {
            next = cur->next;
            free(cur);
            cur = next;
        } while (cur != list->head);
        list->head = NULL;
        list->size = 0;
    }
}

void ScoreList_push_back(ScoreList *list, const char *name, unsigned attempts, game_error_t *e)
{
    if (!list)
        goto NullPtr;

    if (!name || strlen(name) == 0)
        goto EmptyName;

    if (strlen(name) >= SCORELIST_NAME_SIZE)
        goto LongName;

    ScoreNode *new_node = create_node(name, attempts, e);

    if (e)
        goto Error;

    if (!list->head)
    {
        list->head = new_node;
    }
    else
    {
        ScoreNode *last = list->head->prev;
        last->next = new_node;
        new_node->prev = last;
        new_node->next = list->head;
        list->head->prev = new_node;
    }
    list->size++;
    goto Success;

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    return;
NullPtr:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "List pointer is NULL", e);
    return;
EmptyName:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "Name cannot be empty", e);
    return;
LongName:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "Name too long", e);
    return;
Error:
    return;
}

void ScoreList_push_forward(ScoreList *list, const char *name, unsigned attempts, game_error_t *e)
{
    if (!list)
        goto NullPtr;

    if (!name || strlen(name) == 0)
        goto EmptyName;

    if (strlen(name) >= SCORELIST_NAME_SIZE)
        goto Error;

    ScoreNode *new_node = create_node(name, attempts, e);
    if (e)
        goto Error;

    if (!list->head)
    {
        list->head = new_node;
    }
    else
    {
        ScoreNode *last = list->head->prev;
        new_node->next = list->head;
        new_node->prev = last;
        list->head->prev = new_node;
        last->next = new_node;
        list->head = new_node;
    }
    list->size++;
    goto Success;

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    return;
NullPtr:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "List pointer is NULL", e);
    return;
EmptyName:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "Name cannot be empty", e);
    return;
LongName:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "Name too long", e);
    return;
Error:
    return;
}

void ScoreList_insert(ScoreList *list, const char *name, unsigned attempts, size_t index, game_error_t *e)
{
    if (!list)
        goto NullPtr;

    if (!name || strlen(name) == 0)
        goto EmptyName;

    if (strlen(name) >= SCORELIST_NAME_SIZE)
        goto Error;

    if (index > list->size)
        goto OutOfRange;

    if (index == 0)
    {
        ScoreList_push_forward(list, name, attempts, e);
        return;
    }

    if (index == list->size)
    {
        ScoreList_push_back(list, name, attempts, e);
        return;
    }

    ScoreNode *new_node = create_node(name, attempts, e);
    if (e)
        goto Error;

    ScoreNode *next_node = get_node_at(list, index);
    if (!next_node)
    {
        free(new_node);
        ERROR_HANDLER___(SCORELIST_INVALID_INDEX, "Index out of range", e);
        goto Error;
    }
    ScoreNode *prev_node = next_node->prev;

    prev_node->next = new_node;
    new_node->prev = prev_node;
    new_node->next = next_node;
    next_node->prev = new_node;

    list->size++;
    goto Success;

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    return;
NullPtr:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "List pointer is NULL", e);
    return;
EmptyName:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "Name cannot be empty", e);
    return;
LongName:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "Name too long", e);
    return;
OutOfRange:
    ERROR_HANDLER___(SCORELIST_INVALID_INDEX, "Index out of range", e);
    return;
Error:
    return;
}

ScoreNode ScoreList_pop_back(ScoreList *list, game_error_t *e)
{
    ScoreNode empty_node = {.name = "", .attempts = 0, .next = NULL, .prev = NULL};
    ScoreNode result = empty_node;

    if (!list)
        goto NullPtr;

    if (!list->head)
        goto EmptyList;

    ScoreNode *last = list->head->prev;
    result = *last;
    result.next = result.prev = NULL;

    if (list->size == 1)
    {
        free(last);
        list->head = NULL;
    }
    else
    {
        ScoreNode *new_last = last->prev;
        new_last->next = list->head;
        list->head->prev = new_last;
        free(last);
    }
    list->size--;
    goto Success;

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    return result;
NullPtr:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "List pointer is NULL", e);
    return empty_node;
EmptyList:
    ERROR_HANDLER___(SCORELIST_EMPTY, "Cannot pop from empty list", e);
Error:
    return empty_node;
}

ScoreNode ScoreList_pop_forward(ScoreList *list, game_error_t *e)
{
    ScoreNode empty_node = {.name = "", .attempts = 0, .next = NULL, .prev = NULL};
    ScoreNode result = empty_node;

    if (!list)
        goto NullPtr;

    if (!list->head)
        goto EmptyList;

    ScoreNode *first = list->head;
    result = *first;
    result.next = result.prev = NULL;

    if (list->size == 1)
    {
        free(first);
        list->head = NULL;
    }
    else
    {
        ScoreNode *new_head = first->next;
        ScoreNode *last = list->head->prev;
        new_head->prev = last;
        last->next = new_head;
        list->head = new_head;
        free(first);
    }
    list->size--;
    goto Success;

Success:
    ERROR_HANDLER___(GAME_SUCCESS, "", e);
    return result;
NullPtr:
    ERROR_HANDLER___(GAME_WRONG_ARG_FORMAT, "List pointer is NULL", e);
    return empty_node;
EmptyList:
    ERROR_HANDLER___(SCORELIST_EMPTY, "Cannot pop from empty list", e);
    return empty_node;
Error:
    return empty_node;
}

ScoreNode *ScoreList_get_node(ScoreList *list, size_t index)
{
    if (!list || !list->head || index >= list->size)
        return NULL;
    return get_node_at(list, index);
}