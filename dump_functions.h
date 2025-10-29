#ifndef DUMP_FUNC
#define DUMP_FUNC

enum ErrorCodes
{
    list_loop_err =        1,
    list_capacity_err =    2,
    list_head_err =        4,
    list_tail_err =        8,
    list_free_err =        16,
    list_size_err =        32,
    list_PZN_err =         64,
    list_next_err =        128,
    list_prev_err =        256,
};

void ListDump(struct StructList* list);

void PrintError(struct StructList* list);

void FillLogFile(char* image_file_name, struct StructList* list, int file_counter);

enum ReturnStatus ListVerifier(struct StructList* list);

char* GetNewImageFileName(int file_counter);

char* GetNewDotCmd(int file_counter);

#define LIST_VERIFAIER(list)                            \
    if (ListVerifier(list) == error) {                  \
        ListDump(list);                                \
        printf("Îøèáêà %d\n", list->err_code);            \
        return error;                                   \
    }                                                   \

#define PRINT_DUMP_LOG(list, message, index)                    \
    fprintf(log_file, message, index);                           \
    fflush(log_file);                                           \
    ListDump(list);                                            \


#endif
