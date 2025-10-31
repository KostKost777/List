#include <TXLib.h>

#include "list_functions.h"
#include "dump_functions.h"
#include "set_get_functions.h"

const char* log_file_name = "";
FILE* log_file = NULL;

static char* GetNewDotCmd(int file_counter)
{
    char str_command[100] = "";

    snprintf(str_command, sizeof(str_command),
            "dot -Tpng image%d.txt -o image%d.png",
             file_counter, file_counter);

    return strdup(str_command);
}

static char* GetNewImageFileName(int file_counter)
{
    char str_file_counter[100] = "";

    snprintf(str_file_counter, sizeof(str_file_counter),
             "image%d.txt", file_counter);

    return strdup(str_file_counter);
}

enum ReturnStatus ListVerifier(struct StructList* list)
{
    assert(list != NULL);

    //список пуст
    if (GetTail(list) == GetHead(list) && GetHead(list) == 0)
        return success;

    if (GetDataEl(list, 0) != CANARY) {

        SetErrCode(list, GetErrCode(list) | list_canary_err);
        return error;
    }

    if (GetCapacity(list) <= 0 || GetCapacity(list) > MAX_CAPACITY) {

        SetErrCode(list, GetErrCode(list) | list_capacity_err);
        return error;
    }

    if (GetHead(list) >= GetCapacity(list) || GetPrevEl(list, GetHead(list)) != 0) {

        SetErrCode(list, GetErrCode(list) | list_head_err);
        return error;
    }

    if (GetTail(list) >= GetCapacity(list) || GetNextEl(list, GetTail(list)) != 0) {

        SetErrCode(list, GetErrCode(list) | list_tail_err);
        return error;
    }



    if (GetFree(list) != 0 && (GetFree(list) >= GetCapacity(list)
        || (GetDataEl(list, GetFree(list)) != PZN
        || GetPrevEl(list, GetFree(list)) != -1))) {

        SetErrCode(list, GetErrCode(list) | list_free_err);
        return error;
    }

    for (int i = 1; i < GetCapacity(list); i++) {

        if (   (GetPrevEl(list, i) == -1 && GetDataEl(list, i) != PZN)
            || (GetPrevEl(list, i) != -1 && GetDataEl(list, i) == PZN)) {

            SetErrCode(list, GetErrCode(list) | list_PZN_err);
            return error;

        }

        if (GetNextEl(list, i) >= GetCapacity(list)) {

            SetErrCode(list, GetErrCode(list) | list_next_err);
            return error;
        }

        if (  (GetPrevEl(list, i) >= 0 && GetPrevEl(list, i) >= GetCapacity(list))
            || GetPrevEl(list, i) < -1) {

            SetErrCode(list, GetErrCode(list) | list_prev_err);
            return error;
        }

        // проверка зациклинности

        if (i == GetHead(list) && GetPrevEl(list, GetNextEl(list, i)) != i) {

            //printf("H1 INDEX: %d\n", i);
            SetErrCode(list, GetErrCode(list) | list_loop_err);
            return error;
        }

        if (i == GetTail(list) && GetNextEl(list, GetPrevEl(list, i)) != i) {

            //printf("H2 INDEX: %d\n", i);
            SetErrCode(list, GetErrCode(list) | list_loop_err);
            return error;
        }

        if (i != GetTail(list) && i != GetHead(list) && GetDataEl(list, i) != PZN
            && (GetNextEl(list, GetPrevEl(list, i)) != i
             || GetPrevEl(list, GetNextEl(list, i)) != i)) {

            //printf("H3 INDEX: %d\n", i);
            SetErrCode(list, GetErrCode(list) | list_loop_err);
            return error;

        }
    }

    int count_el = 0;

    for (int i = GetHead(list); i != 0; i = GetNextEl(list, i))
        count_el++;

    //printf("COUNTER: %d\n", count_el);

    if (GetNumOfEl(list) != count_el) {
        SetErrCode(list, GetErrCode(list) | list_num_of_el_err);
        return error;
    }

    count_el = 0;

    for (int i = GetTail(list); i != 0; i = GetPrevEl(list, i))
        count_el++;

    if (GetNumOfEl(list) != count_el) {
        SetErrCode(list, GetErrCode(list) | list_num_of_el_err);
        return error;
    }

    return success;
}

void PrintError(struct StructList* list)
{
    assert(list != NULL);

    if (GetErrCode(list) & list_loop_err)
        fprintf(log_file, "<h2>BAD_LIST_LOOP - [%d]</h2>\n", list_loop_err);

    if (GetErrCode(list) & list_capacity_err)
        fprintf(log_file, "<h2>BAD_CAPACITY - [%d]</h2>\n", list_capacity_err);

    if (GetErrCode(list) & list_head_err)
        fprintf(log_file, "<h2>BAD_HEAD - [%d]</h2>\n", list_head_err);

    if (GetErrCode(list) & list_tail_err)
        fprintf(log_file, "<h2>BAD_TAIL - [%d]</h2>\n", list_tail_err);

    if (GetErrCode(list) & list_free_err)
        fprintf(log_file, "<h2>BAD_FREE - [%d]</h2>\n", list_free_err);

    if (GetErrCode(list) & list_PZN_err)
        fprintf(log_file, "<h2>BAD_PZN - [%d]</h2>\n", list_PZN_err);

    if (GetErrCode(list) & list_next_err)
        fprintf(log_file, "<h2>BAD_NEXT - [%d]</h2>\n", list_next_err);

    if (GetErrCode(list) & list_prev_err)
        fprintf(log_file, "<h2>BAD_PREV - [%d]</h2>\n", list_loop_err);

    if (GetErrCode(list) & list_canary_err)
        fprintf(log_file, "<h2>BAD_CANARY - [%d]</h2>\n", list_canary_err);

    if (GetErrCode(list) & list_num_of_el_err)
        fprintf(log_file, "<h2>BAD_NUM_OF_ELEMENT - [%d]</h2>\n", list_num_of_el_err);


}

enum ReturnStatus ListDump(struct StructList* list,
                           const int line, const char* func, const char* file)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    if (GetErrCode(list) != 0) {
        PrintError(list);
        return fatal_error;
    }

    fprintf(log_file, "<h4>LIST { %s %s:%d }</h4>\n", func, file, line);

    static int file_counter = 0;

    char* image_file_name = GetNewImageFileName(file_counter);

    FILE* graphiz_file = fopen(image_file_name, "w");

    fprintf(graphiz_file, "digraph {\n"
                          "rankdir = HR;\n"
                          "splines = ortho\n"
                          "ranksep = 1.0\n"
                          "nodesep = 0.5\n");

    fprintf(graphiz_file, "head "
                          "[shape = Mrecord; "
                          "style = filled; "
                          "fillcolor = \"#F2B138\"; "
                          "color = \"#80000\"; "
                          "label = \"HEAD\"; ]\n");

    fprintf(graphiz_file, "tail "
                          "[shape = Mrecord; "
                          "style = filled; "
                          "fillcolor = \"#5F85BB\"; "
                          "color = \"#80000\"; "
                          "label = \"TAIL\"; ]\n");

    fprintf(graphiz_file, "free "
                          "[shape = Mrecord; "
                          "style = filled; "
                          "fillcolor = \"#F14065\"; "
                          "color = \"#80000\"; "
                          "label = \"FREE\"; ]\n");

    const char* color = NULL;

    for (int i = 0; i < GetCapacity(list); i++) {

        bool is_free_index = false;

        for (int index_free_el = GetFree(list);
                 index_free_el != 0;
                 index_free_el = GetNextEl(list, index_free_el)) {

                 if (i == index_free_el) {
                    is_free_index = true;
                    break;
                }
        }

        if (i == 0)
            color = "#F984E5";

        else if (is_free_index)

            color = "#F14065";

        else if(i == GetHead(list))

            color = "#F2B138";

        else if (i == GetTail(list))

            color = "#5F85BB";

        else

            color = "#77DD77";

        fprintf(graphiz_file, "node%d "
                              "[shape = Mrecord; "
                              "style = filled; "
                              "fillcolor = \"%s\"; "
                              "color = \"#00000\"; "
                              "label = \"{INDEX = %d |",
                               i, color, i);

        if (GetDataEl(list, i) == PZN)
            fprintf(graphiz_file, "DATA = PZN |");

        else
            fprintf(graphiz_file, "DATA = %d |", GetDataEl(list, i));

        fprintf(graphiz_file, "{NEXT = %d |"
                              "PREV = %d  }}"
                              "\"; ]\n",
                              GetNextEl(list, i),
                              GetPrevEl(list, i));

    }

    fprintf(graphiz_file, "{ rank = same; ");

    for (int i = 0; i < GetCapacity(list); ++i) {
        fprintf(graphiz_file, "node%d; ", i);
    }

    fprintf(graphiz_file, "}\n");

    fprintf(graphiz_file, "{ rank = same; head; tail; free}\n");

    for (int i = 0; i < GetCapacity(list) - 1; i++) {
        fprintf(graphiz_file, "node%d -> node%d "
                              "[color = \"transparent\";"
                              " weight=10]\n", i, i + 1);
    }

    for (int i = GetHead(list); i != GetTail(list); i = GetNextEl(list, i)) {

        if (i == GetPrevEl(list, GetNextEl(list, i)))
            fprintf(graphiz_file, "node%d -> node%d "
                          "[color = \"black\", "
                          "dir = both, "
                          " weight=0]\n", i, GetNextEl(list, i));

    }

    /*for (int i = list->free; i != 0; i = list->next[i])

        fprintf(graphiz_file, "node%d -> node%d "
                              "[color = \"#F14065\", "
                              " weight=0]\n", i, list->next[i]);*/

    fprintf(graphiz_file, "node%d -> node0 "
                          "[color = \"#CD00CD\","
                          " weight=0]\n", GetTail(list));

    fprintf(graphiz_file, "node0 -> node%d "
                          "[color = \"#CD00CD\","
                          " weight=0]\n", GetHead(list));


    fprintf(graphiz_file, "head -> node%d "
                          "[color = \"#F2B138\","
                          " weight=10]\n", GetHead(list));

    fprintf(graphiz_file, "tail -> node%d "
                          "[color = \"#5F85BB\","
                          " weight=10]\n", GetTail(list));

    fprintf(graphiz_file, "free -> node%d "
                          "[color = \"#F14065\","
                          " weight=10]\n", GetFree(list));

    fprintf(graphiz_file, "}");

    fclose(graphiz_file);
    free(image_file_name);

    char* command = GetNewDotCmd(file_counter);
    //printf("%s\n", command);
    system(command);
    free(command);

    FillLogFile(image_file_name, list, file_counter);

    file_counter++;

    return success;
}

void FillLogFile(char* image_file_name, struct StructList* list, int file_counter)
{
    assert(image_file_name != NULL);
    assert(list != NULL);

    fprintf(log_file, "HEAD: %d\n", GetHead(list));
    fprintf(log_file, "TAIL: %d\n", GetTail(list));
    fprintf(log_file, "FREE: %d\n\n", GetFree(list));

    fprintf(log_file, "INDX: ");

    for (int i = 0; i < GetCapacity(list); ++i) {
        fprintf(log_file, "|%3d| ", i);
    }

    fprintf(log_file, "\nDATA: ");

    for (int i = 0; i < GetCapacity(list); ++i) {
        fprintf(log_file, "|%3d| ", GetDataEl(list, i));
    }

    fprintf(log_file, "\nNEXT: ");

    for (int i = 0; i < GetCapacity(list) ; ++i) {
        fprintf(log_file, "|%3d| ", GetNextEl(list, i));
    }

    fprintf(log_file, "\nPREV: ");

    for (int i = 0; i < GetCapacity(list) ; ++i) {
        fprintf(log_file, "|%3d| ", GetPrevEl(list, i));
    }

    fprintf(log_file, "\n\n<img src=image%d.png width=%dpx>\n\n",
                                                    file_counter,
                                                    GetCapacity(list) * 200);

    fprintf(log_file, "-------------------------------------------------------\n");

    fflush(log_file);
}

void PrintDumpLog(struct StructList* list,
                   const int line, const char* func, const char* file,
                   const char* message, ...)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);
    assert(message != NULL);

    va_list args;
    va_start(args, message);

    vfprintf(log_file, message, args);
    fflush(log_file);

    va_end(args);

    ListDump(list, line, func, file);
}
