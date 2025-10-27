#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"

ReturnStatus ListCtor(struct StructList* list)
{
    assert(list != NULL);

    list->data = (int*)calloc(list->capacity, sizeof(int));

    list->next = (int*)calloc(list->capacity, sizeof(int));

    list->prev = (int*)calloc(list->capacity, sizeof(int));

    if (list->data == NULL || list->next == NULL || list->prev == NULL) {
        printf("Memory was not allocated");
        errno = error;
        return error;
    }

    SetDefaultNext(list);
    SetDefaultPrev(list);

    list->head = 1;
    list->tail = 1;
    list->free = 1;

    return success;
}

void ListDtor(struct StructList* list)
{
    assert(list != NULL);

    free(list->data);
    free(list->next);
    free(list->prev);

    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;
}

void SetDefaultNext(struct StructList* list)
{
    assert(list != NULL);

    list->next[0] = 0;

    for (size_t i = 1; i < list->capacity; ++i)
        list->next[i] = i + 1;

    list->next[list->capacity - 1] = 0;

}

void SetDefaultPrev(struct StructList* list)
{
    assert(list != NULL);

    list->prev[0] = 0;

    for (size_t i = 1; i < list->capacity; ++i)
        list->prev[i] = -1;

}

enum ReturnStatus InsertAfter(struct StructList* list,
                              size_t prev_index, int value)
{
    assert(list != NULL);

    if (list->tail == list->capacity - 1) {
        printf("List is full");
        return error;
    }

    printf("\nпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ %u\n", prev_index);

    // вставка первого элемента
    if (prev_index == 0 && list->tail == 1) {

        list->data[list->free] = value;
        list->prev[list->free] = 0;

        list->free++;
    }

    // вставка нового первого элемента

    else if (prev_index == 0) {

        list->data[list->free] = value;

        list->prev[list->free] = list->prev[list->head];
        list->prev[list->head] = list->free;

        int twin_head = list->head;
        int twin_free = list->free;

        list->head = list->free;
        list->free = list->next[list->free];
        list->next[twin_free] = twin_head;

    }

    //вставка в конец
    else if (list->tail == prev_index) {

        list->tail++;

        list->data[list->free] = value;

        list->next[prev_index] = list->free;

        list->prev[list->free] = prev_index;

        list->free = list->next[list->free];

        list->next[list->tail] = 0;

    }

    //вставка между элементами

    else {

        list->data[list->free] = value;

        int twin_free = list->free;

        list->free = list->next[list->free];

        list->next[twin_free] = list->next[prev_index];

        list->prev[list->next[prev_index]] = twin_free;

        list->next[prev_index] = twin_free;

        list->prev[twin_free] = prev_index;

    }

    return success;
}

enum ReturnStatus DeleteElement(struct StructList* list, size_t del_index)
{
    assert(list != NULL);

    //список пуст
    if (list->tail == 1) {
        printf("List is empty");
        return error;
    }

    //удаление с конца
    if (del_index == list->tail) {

        list->data[del_index] = 0;

        list->tail = list->prev[del_index];

        list->next[list->prev[del_index]] = 0;
        list->next[del_index] = list->free;
        list->free = del_index;

        list->prev[del_index] = -1;

        return success;
    }

    //удаление первого элемента
    if (del_index == list->head) {
       list->data[del_index] = 0;

       list->head = list->next[del_index];

       list->next[del_index] = list->free;
       list->free = del_index;

       list->prev[del_index] = -1;

       return success;
    }

    list->data[del_index] = 0;
    list->next[list->prev[del_index]] = list->next[del_index];

    list->prev[list->next[del_index]] = list->prev[del_index];

    list->prev[del_index] = -1;

    list->next[del_index] = list->free;
    list->free = del_index;

    return success;
}

void ListDump(struct StructList* list)
{
    assert(list != NULL);

    printf("\nDATA: \n");
    for (size_t i = 0; i < list->capacity; ++i) {
        printf("%d ", list->data[i]);
    }

    printf("\nNEXT: \n");
    for (size_t i = 0; i < list->capacity; ++i) {
        printf("%d ", list->next[i]);
    }

    printf("\nPREV: \n");
    for (size_t i = 0; i < list->capacity; ++i) {
        printf("%d ", list->prev[i]);
    }

    printf("\nHEAD: %u", list->head);

    printf("\nTAIL: %u", list->tail);

    printf("\nFREE: %u\n", list->free);
}






