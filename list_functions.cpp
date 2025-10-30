#include <TXLib.h>

#include "list_functions.h"
#include "dump_functions.h"

static void SetDefaultNext(struct StructList* list)
{
    assert(list != NULL);

    list->next[0] = 0;

    for (int i = 1; i < list->capacity; ++i)
        list->next[i] = i + 1;

    list->next[list->capacity - 1] = 0;

}

static void SetDefaultData(struct StructList* list)
{
    assert(list != NULL);

    list->data[0] = CANARY;

    for (int i = 1; i < list->capacity; ++i)
        list->data[i] = PZN;

}

static void SetDefaultPrev(struct StructList* list)
{
    assert(list != NULL);

    list->prev[0] = 0;

    for (int i = 1; i < list->capacity; ++i)
        list->prev[i] = -1;

}

ReturnStatus ListCtor(struct StructList* list, int capacity)
{
    assert(list != NULL);

    list->capacity = capacity;

    list->data = (int*)calloc(list->capacity, sizeof(int));

    list->next = (int*)calloc(list->capacity, sizeof(int));

    list->prev = (int*)calloc(list->capacity, sizeof(int));

    if (list->data == NULL || list->next == NULL || list->prev == NULL) {
        printf("Memory was not allocated");
        return error;
    }

    SetDefaultNext(list);
    SetDefaultPrev(list);
    SetDefaultData(list);

    list->num_of_el = 0;
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

int InsertAfter(struct StructList* list,
                int index, int value,
                const int line, const char* func, const char* file)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    LIST_VERIFIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: Before InsertAfter(%d, %d)</h3>\n",
                                                           index, value);

    if (   index < 0
        || list->prev[index] == -1
        || list->prev[index] >= list->capacity) {

        fprintf(log_file, "<h2>This element with index(%d) did not exist</h2>\n",
                                                                          index);
        return error;
    }

    if (list->num_of_el >= list->capacity - 1) {

        fprintf(log_file, "<h2>Not enough capacity</h2>\n");
        return error;
    }

    list->data[list->free] = value;

    int old_free = list->free;

    list->free = list->next[list->free];

    list->next[old_free] = list->next[index];

    list->prev[list->next[index]] = old_free;

    list->next[index] = old_free;

    list->prev[old_free] = index;

    list->num_of_el++;

    LIST_VERIFIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After InsertAfter(%d, %d)</h3>\n",
                                                           index, value);

    return old_free;
}

int InsertBefore(struct StructList* list,
                 int index, int value,
                 const int line, const char* func, const char* file)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    LIST_VERIFIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: Before InsertBefore(%d, %d)</h3>\n",
                                                            index, value);

    if (   index < 0
        || list->prev[index] == -1
        || list->prev[index] >= list->capacity) {

        fprintf(log_file, "<h2>This element with index(%d) did not exist</h2>\n",
                                                                      index);
        return error;
    }

    if (list->num_of_el >= list->capacity - 1) {

        fprintf(log_file, "<h2>Not enough capacity</h2>\n");
        return error;
    }

    list->data[list->free] = value;

    int old_free = list->free;

    list->free = list->next[list->free];

    list->next[list->prev[index]] = old_free;

    list->next[old_free] = index;

    list->prev[old_free] = list->prev[index];

    list->prev[index] = old_free;

    list->num_of_el++;

    LIST_VERIFIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After InsertBefore(%d, %d)</h3>\n",
                                                           index, value);
    return old_free;
}

enum ReturnStatus DeleteElement(struct StructList* list,
                                int del_index,
                                const int line, const char* func, const char* file)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    LIST_VERIFIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: Before Delete(%d)</h3>\n", del_index);

    if (   del_index <= 0
        || list->prev[del_index] == -1
        || list->prev[del_index] >= list->capacity) {

        fprintf(log_file, "<h2>This element with index(%d) did not exist</h2>\n",
                                                                      del_index);
        return error;
    }

    list->data[del_index] = PZN;

    list->next[list->prev[del_index]] = list->next[del_index];

    list->prev[list->next[del_index]] = list->prev[del_index];

    list->prev[del_index] = -1;

    list->next[del_index] = list->free;

    list->free = del_index;

    list->num_of_el--;

    LIST_VERIFIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After Delete(%d)</h3>\n", del_index);

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

enum ReturnStatus OpenLogFile()
{
    log_file_name = "list_logs.html";

    log_file = fopen(log_file_name, "w");

    if (log_file == NULL)
        return error;

    printf("Logfile open success\n");

    fprintf(log_file, "<pre>\n");

    return success;
}






