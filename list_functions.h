#ifndef LIST_FUNC
#define LIST_FUNC

const int PZN = 228;

const size_t MAX_CAPACITY = 1E6;

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
    size_t head;
    size_t tail;
    size_t free;
    size_t capacity;
    int err_code;
};

extern const char* log_file_name;

extern FILE* log_file;

enum ReturnStatus ListCtor (struct StructList* list);

void ListDtor(struct StructList* list);

void UserPrintList(struct StructList* list);

void SetDefaultNext(struct StructList* list);

void SetDefaultPrev(struct StructList* list);

void SetDefaultData(struct StructList* list);

enum ReturnStatus InsertAfter(struct StructList* list,
                              size_t prev_index, int value,
                              const int LINE, const char* FUNC, const char* FILE);

enum ReturnStatus DeleteElement(struct StructList* list,
                                size_t del_index,
                                const int LINE, const char* FUNC, const char* FILE);

void CloseLogFile();

#endif
