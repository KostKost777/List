#ifndef LIST_FUNC
#define LIST_FUNC

const int PZN = 228;

const int MAX_CAPACITY = 1E6;

const int CANARY = 999;

enum ReturnStatus
{
    success = 0,
    error = 1,
    fatal_error = 2,
};

struct StructList
{
    int* data;
    int* next;
    int* prev;
    int free;
    int capacity;
    size_t num_of_el;
    int err_code;
};

enum ReturnStatus ListCtor (struct StructList* list);

void ListDtor(struct StructList* list);

void UserPrintList(struct StructList* list);

void SetDefaultNext(struct StructList* list);

void SetDefaultPrev(struct StructList* list);

void SetDefaultData(struct StructList* list);

enum ReturnStatus InsertAfter(struct StructList* list,
                              int prev_index, int value,
                              const int LINE, const char* FUNC, const char* FILE);

enum ReturnStatus InsertBefore(struct StructList* list,
                              int index, int value,
                              const int line, const char* func, const char* file);

enum ReturnStatus DeleteElement(struct StructList* list,
                                int del_index,
                                const int LINE, const char* FUNC, const char* FILE);

enum ReturnStatus OpenLogFile();

void CloseLogFile();

#define INSERT_AFTER(list, index, element)                        \
    if ((errno = InsertAfter(list, index, element,                          \
                    __LINE__, __func__, __FILE__)) == error) {      \
        goto exit;                                                \
    }

#define INSERT_BEFORE(list, index, element)                        \
    if ((errno = InsertBefore(list, index, element,                          \
                    __LINE__, __func__, __FILE__)) == error) {      \
        goto exit;                                                \
    }

#define DELETE_ELEMENT(list, index)                        \
    if ((errno = DeleteElement(list, index,                          \
                    __LINE__, __func__, __FILE__)) == error) {      \
        goto exit;                                                \
    }

#endif
