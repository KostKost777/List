#ifndef LIST_FUNC
#define LIST_FUNC

enum ReturnStatus
{
    success = 0,
    error = 1
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
};

extern const char* log_file_name;

enum ReturnStatus ListCtor (struct StructList* list);

void ListDtor(struct StructList* list);

char* GetNewImageFileName(int file_counter);

char* GetNewDotCmd(int file_counter);

void UserPrintList(struct StructList list);

// enum ReturnStatus ListVerifier(struct StructList* list);

void SetDefaultNext(struct StructList* list);

void SetDefaultPrev(struct StructList* list);

void ListDump(struct StructList list);

void FillLogFile(char* image_file_name, struct StructList list, int file_counter);

enum ReturnStatus InsertAfter(struct StructList* list,
                              size_t prev_index, int value);

enum ReturnStatus DeleteElement(struct StructList* list,
                                size_t del_index);

#define INSERT_AFTER(list, index)  \
    InsertAfter(list, index, __LINE__, __func__, __FILE__); \

#endif
