#include <TXLib.h>

#include "list_functions.h"
#include "dump_functions.h"
#include "set_get_functions.h"

//  GET
int GetHead(struct StructList* list)
{
    assert(list);
    return list->next[0];
}

int GetTail(struct StructList* list)
{
    assert(list);
    return list->prev[0];
}

int GetFree(struct StructList* list)
{
    assert(list);
    return list->free;
}

int GetNumOfEl(struct StructList* list)
{
    assert(list);
    return list->num_of_el;
}

int GetCapacity(struct StructList* list)
{
    assert(list);
    return list->capacity;
}

int GetDataEl(struct StructList* list, int index)
{
    assert(list);
    return list->data[index];
}

int GetPrevEl(struct StructList* list, int index)
{
    assert(list);
    return list->prev[index];
}

int GetNextEl(struct StructList* list, int index)
{
    assert(list);
    return list->next[index];
}

int GetErrCode(struct StructList* list)
{
    assert(list);
    return list->err_code;
}


//SET

void SetHead(struct StructList* list, int value)
{
    assert(list);
    list->next[0] = value;
}

void SetTail(struct StructList* list, int value)
{
    assert(list);
    list->prev[0] = value;
}

void SetFree(struct StructList* list, int value)
{
    assert(list);
    list->free = value;
}

void SetNumOfEl(struct StructList* list, int value)
{
    assert(list);
    list->num_of_el = value;
}

void SetCapacity(struct StructList* list, int value)
{
    assert(list);
    list->capacity = value;
}

void SetDataEl(struct StructList* list, int index, int value)
{
    assert(list);
    list->data[index] = value;
}

void SetPrevEl(struct StructList* list, int index, int value)
{
    assert(list);
    list->prev[index] = value;
}

void SetNextEl(struct StructList* list, int index, int value)
{
    assert(list);
    list->next[index] = value;
}

void SetErrCode(struct StructList* list, int value)
{
    assert(list);
    list->err_code = value;
}

