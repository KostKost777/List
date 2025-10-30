#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"
#include "dump_functions.h"

const char* log_file_name = "list_logs.html";

FILE* log_file = fopen(log_file_name, "w");

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
    SetDefaultData(list);

    list->free = 1;
    list->err_code = 0;

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

    for (int i = 1; i < list->capacity; ++i)
        list->next[i] = i + 1;

    list->next[list->capacity - 1] = 0;

}

void SetDefaultData(struct StructList* list)
{
    assert(list != NULL);

    list->data[0] = CANARY;

    for (int i = 1; i < list->capacity; ++i)
        list->data[i] = PZN;

}

void SetDefaultPrev(struct StructList* list)
{
    assert(list != NULL);

    list->prev[0] = 0;

    for (int i = 1; i < list->capacity; ++i)
        list->prev[i] = -1;

}

enum ReturnStatus InsertAfter(struct StructList* list,
                              int prev_index, int value,
                              const int line, const char* func, const char* file)
{
    assert(list != NULL);

    LIST_VERIFAIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: Before InsertAfter [%d]</h3>\n", prev_index);

    list->data[list->free] = value;

    int twin_free = list->free;

    list->free = list->next[list->free];

    list->next[twin_free] = list->next[prev_index];

    list->prev[list->next[prev_index]] = twin_free;

    list->next[prev_index] = twin_free;

    list->prev[twin_free] = prev_index;

    LIST_VERIFAIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After InsertAfter [%d]</h3>\n", prev_index);

    return success;
}

enum ReturnStatus DeleteElement(struct StructList* list,
                                int del_index,
                                const int line, const char* func, const char* file)
{
    assert(list != NULL);

    LIST_VERIFAIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: Before Delete [%d]</h3>\n", del_index);

    list->data[del_index] = PZN;

    list->next[list->prev[del_index]] = list->next[del_index];

    list->prev[list->next[del_index]] = list->prev[del_index];

    list->prev[del_index] = -1;

    list->next[del_index] = list->free;

    list->free = del_index;

    LIST_VERIFAIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After Delete [%d]</h3>\n", del_index);

    return success;
}

void UserPrintList(struct StructList* list)
{
    for (int i = list->next[0]; i != 0; i = list->next[i]) {
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

void CloseLogFile()
{
    fclose(log_file);
    printf("Logfile close\n");
}






