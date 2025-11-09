#include <TXLib.h>

#include "list_functions.h"
#include "dump_functions.h"
#include "set_get_functions.h"

static void SetDefaultNext(struct StructList* list)
{
    assert(list != NULL);

    SetHead(list, 0);

    for (int i = 1; i < GetCapacity(list); ++i)
        SetNextEl(list, i, i + 1);

    SetNextEl(list, GetCapacity(list) - 1, 0);

}

static void SetDefaultData(struct StructList* list)
{
    assert(list != NULL);

    SetDataEl(list, 0, CANARY);

    for (int i = 1; i < GetCapacity(list); ++i)
        SetDataEl(list, i, PZN);

}

static void SetDefaultPrev(struct StructList* list)
{
    assert(list != NULL);

    SetTail(list, 0);

    for (int i = 1; i < GetCapacity(list); ++i)
        SetPrevEl(list, i, -1);

}

ReturnStatus ListCtor(struct StructList* list, int capacity)
{
    assert(list != NULL);

    SetCapacity(list, capacity);

    list->data = (int*)calloc(GetCapacity(list), sizeof(int));

    list->next = (int*)calloc(GetCapacity(list), sizeof(int));

    list->prev = (int*)calloc(GetCapacity(list), sizeof(int));

    if (list->data == NULL || list->next == NULL || list->prev == NULL) {
        printf("Memory was not allocated");
        return error;
    }

    SetDefaultNext(list);
    SetDefaultPrev(list);
    SetDefaultData(list);

    SetNumOfEl(list, 0);
    SetFree(list, 1);
    SetErrCode(list, 0);

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

int Insert(struct StructList* list, int index, int value)
{
    if (   index < 0
        || GetPrevEl(list, index) == -1
        || GetPrevEl(list, index) >= GetCapacity(list)) {

        fprintf(log_file, "<h2>This element with index(%d) did not exist</h2>\n",
                                                                          index);
        return error;
    }

    if (GetNumOfEl(list) >= GetCapacity(list) - 1) {
        if (UpwardReallocate(list) == error)
            return error;
    }

    SetDataEl(list, GetFree(list), value);

    int old_free = GetFree(list);

    SetFree(list, GetNextEl(list, GetFree(list)));

    SetNextEl(list, old_free, GetNextEl(list, index));

    SetPrevEl(list, GetNextEl(list, index), old_free);

    SetNextEl(list, index, old_free);

    SetPrevEl(list, old_free, index);

    SetNumOfEl(list, GetNumOfEl(list) + 1);

    return old_free;
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
    int ret_value = Insert(list, index, value);

    LIST_VERIFIER(list);
    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After InsertAfter(%d, %d)</h3>\n",
                                                           index, value);

    return ret_value;
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

    int ret_value = Insert(list, GetPrevEl(list, index), value);

    LIST_VERIFIER(list);
    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After InsertBefore(%d, %d)</h3>\n",
                                                           index, value);

    return ret_value;
}

int InsertBeforeHead(struct StructList* list,
                     int value,
                     const int line, const char* func, const char* file)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    LIST_VERIFIER(list);
    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: Before InsertBeforeHead(%d)</h3>\n",
                                                           value);

    int ret_value = Insert(list, GetPrevEl(list, GetHead(list)), value);

    LIST_VERIFIER(list);
    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After InsertBeforeHead(%d)</h3>\n",
                                                           value);

    return ret_value;
}

int InsertAfterTail(struct StructList* list,
                     int value,
                     const int line, const char* func, const char* file)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    LIST_VERIFIER(list);
    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: Before InsertAfterTail(%d)</h3>\n",
                                                           value);

    int ret_value = Insert(list, GetTail(list), value);

    LIST_VERIFIER(list);
    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After InsertAfterTail(%d)</h3>\n",
                                                           value);

    return ret_value;
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

    if (   del_index < 0
        || GetPrevEl(list, del_index) == -1
        || GetPrevEl(list, del_index) >= GetCapacity(list)) {

        fprintf(log_file, "<h2>This element with index(%d) did not exist</h2>\n",
                                                                      del_index);
        return error;
    }

    SetDataEl(list, del_index, PZN);

    SetNextEl(list, GetPrevEl(list, del_index), GetNextEl(list, del_index));
    SetPrevEl(list, GetNextEl(list, del_index), GetPrevEl(list, del_index));

    SetPrevEl(list, del_index, -1);

    SetNextEl(list, del_index, list->free);

    SetFree(list, del_index);

    SetNumOfEl(list, GetNumOfEl(list) - 1);

    LIST_VERIFIER(list);

    PRINT_DUMP_LOG(list, "\n<h3>\nDUMP: After Delete(%d)</h3>\n", del_index);

    return success;
}

int UserPrintList(struct StructList* list)
{
    if (!ListVerifier) {
        ListDump(list, __LINE__, __func__, __FILE__);
        return error;
    }

    for (int i = GetHead(list); i != 0; i = GetNextEl(list, i)) {
        printf("%d ", GetDataEl(list, i));
    }
    printf("\n");

    return success;

}

enum ReturnStatus Linearization(struct StructList* list)
{
    assert(list);

    fprintf(log_file,
        "<\h3>Warning: After linearization you old index may be invalid<h3>\n");

    int free_index = -1;
    bool is_first_free = true;

    for (int i = 1; i < GetCapacity(list); ++i) {

        if (GetDataEl(list, i) == PZN && is_first_free) {
            free_index = i;
            is_first_free = false;
        }

        if (GetDataEl(list, i) != PZN && !is_first_free) {

            SetDataEl(list, free_index, GetDataEl(list, i));
            SetNextEl(list, free_index, GetNextEl(list, i));
            SetPrevEl(list, free_index, GetPrevEl(list, i));

            SetNextEl(list, GetPrevEl(list, i), free_index);
            SetPrevEl(list, GetNextEl(list, i), free_index);

            free_index++;
        }
    }

    if (free_index == -1)
        return success;

    for (int i = GetNumOfEl(list) + 1; i < GetCapacity(list); ++i) {

        SetDataEl(list, i, PZN);
        SetNextEl(list, i, i + 1);
        SetPrevEl(list, i, -1);
    }

    SetNextEl(list, GetCapacity(list) - 1, 0);

    SetFree(list, 0);

    return success;

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

enum ReturnStatus UpwardReallocate(struct StructList* list)
{
    assert(list != NULL);

    int* new_data = (int*)realloc(list->data, sizeof(int) * GetCapacity(list) * 2);
    int* new_next = (int*)realloc(list->next, sizeof(int) * GetCapacity(list) * 2);
    int* new_prev = (int*)realloc(list->prev, sizeof(int) * GetCapacity(list) * 2);

    if (new_data == NULL || new_next == NULL || new_prev == NULL) {
        free(new_data);
        free(new_next);
        free(new_prev);

        new_data = NULL;
        new_next = NULL;
        new_prev = NULL;

        fprintf(log_file, "<h2> Memory did not allocate </h2>\n");

        return error;
    }

    list->data = new_data;
    list->next = new_next;
    list->prev = new_prev;

    for (int i = GetCapacity(list); i < 2 * GetCapacity(list); ++i) {
        SetDataEl(list, i, PZN);
    }

    for (int i = GetCapacity(list); i < 2 * GetCapacity(list); ++i) {
        SetNextEl(list, i, i + 1);
    }

    SetNextEl(list, 2 * GetCapacity(list) - 1, 0);

    SetFree(list, GetCapacity(list));

    for (int i = GetCapacity(list); i < 2 * GetCapacity(list); ++i) {
        SetPrevEl(list, i, -1);
    }

    SetCapacity(list, 2 * GetCapacity(list));

    return success;
}

enum ReturnStatus DownwardReallocate(struct StructList* list, bool with_linearization)
{
    assert(list != NULL);

    if (with_linearization)
        Linearization(list);

    int counter = 0;

    for (int i = GetCapacity(list) - 1; GetDataEl(list, i) == PZN; --i)
        counter++;

    //printf("CAPA: %d\n", GetCapacity(list) - 1);

    if (counter == 0)
        return error;

    int index = 1;

    SetCapacity(list, GetCapacity(list) - counter);

    //printf("CAPA: %d\n", GetCapacity(list));

    //это для обновления фри

    if (!with_linearization) {

        for (; GetNextEl(list, index) < GetCapacity(list); index++);

        SetNextEl(list, index, 0);

    }

    int* new_data = (int*)realloc(list->data, sizeof(int) * GetCapacity(list));
    int* new_next = (int*)realloc(list->next, sizeof(int) * GetCapacity(list));
    int* new_prev = (int*)realloc(list->prev, sizeof(int) * GetCapacity(list));

    if (new_data == NULL || new_next == NULL || new_prev == NULL) {

        free(new_data);
        free(new_next);
        free(new_prev);

        new_data = NULL;
        new_next = NULL;
        new_prev = NULL;

        fprintf(log_file, "<h2> Memory did not allocate </h2>\n");

        return error;
    }

    list->data = new_data;
    list->next = new_next;
    list->prev = new_prev;

    return success;
}

int SortListByNext(struct StructList* list)
{
    assert(list != NULL);

    if (!ListVerifier) {
        ListDump(list, __LINE__, __func__, __FILE__);
        return error;
    }

    int end_index = 0;
    int next_el = 0;

    for (int i = GetHead(list); i != 0; i = GetNextEl(list, i)) {
        for (int j = GetHead(list); j != end_index; j = next_el) {

            //printf("I: %d   J: %d \n", i, j);
            next_el = GetNextEl(list, j);

            if (   GetDataEl(list, GetNextEl(list, j)) != CANARY
                && j > GetNextEl(list, j) ) {

                next_el = GetPrevEl(list, j);

                SwapNode(list, j, GetNextEl(list, j));

                //ListDump(list, __LINE__, __func__, __FILE__);
                //getchar();

            }
        }
        end_index = GetPrevEl(list, end_index);
    }

    return success;
}

void SwapNode(struct StructList* list, int ind1, int ind2)
{
    assert(list != NULL);

    int twin_data = GetDataEl(list, ind1);
    SetDataEl(list, ind1, GetDataEl(list, ind2));
    SetDataEl(list, ind2, twin_data);

    int twin_next = GetNextEl(list, ind2);

    SetPrevEl(list, GetNextEl(list, ind2), ind1);
    SetNextEl(list, GetPrevEl(list, ind1), ind2);

    SetNextEl(list, ind2, ind1);
    SetPrevEl(list, ind2, GetPrevEl(list, ind1));

    SetNextEl(list, ind1, twin_next);
    SetPrevEl(list, ind1, ind2);

}
