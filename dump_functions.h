#ifndef DUMP_FUNC
#define DUMP_FUNC

extern const char* log_file_name;

extern FILE* log_file;

enum ErrorCodes
{
    list_loop_err =        1,
    list_capacity_err =    2,
    list_head_err =        4,
    list_tail_err =        8,
    list_free_err =        16,
    list_num_of_el_err =   32,
    list_PZN_err =         64,
    list_next_err =        128,
    list_prev_err =        256,
    list_canary_err =      512,
};

enum ReturnStatus ListDump(struct StructList* list,
                           const int LINE, const char* FUNC, const char* FILE);

void PrintError(struct StructList* list);

void FillLogFile(char* image_file_name, struct StructList* list, int file_counter);

enum ReturnStatus ListVerifier(struct StructList* list);

void PrintDumpLog(struct StructList* list,
                   const int line, const char* func, const char* file,
                   const char* message, ...);

#define LIST_VERIFIER(list)                            \
    if (ListVerifier(list) == error) {                  \
        printf("Îøèáêà %d\n", list->err_code);            \
        ListDump(list, line, func, file);                \
        return error;                                   \
    }                                                \

#define PRINT_DUMP_LOG(list, message, ...)                    \
        PrintDumpLog(list, line, func, file, message, ##__VA_ARGS__);     \

#endif
