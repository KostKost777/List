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

    return strdup(str_command);     // можно без strdup by VB
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

    }

    if (GetCapacity(list) <= 0 || GetCapacity(list) > MAX_CAPACITY) {

        SetErrCode(list, GetErrCode(list) | list_capacity_err);
        return error;
    }

    if (GetHead(list) >= GetCapacity(list) || GetPrevEl(list, GetHead(list)) != 0) {

        SetErrCode(list, GetErrCode(list) | list_head_err);

    }

    if (GetTail(list) >= GetCapacity(list) || GetNextEl(list, GetTail(list)) != 0) {

        SetErrCode(list, GetErrCode(list) | list_tail_err);

    }

    if (GetFree(list) != 0 && (GetFree(list) >= GetCapacity(list)
        || (GetDataEl(list, GetFree(list)) != PZN
        || GetPrevEl(list, GetFree(list)) != -1))) {

        SetErrCode(list, GetErrCode(list) | list_free_err);

    }

    for (int i = 1; i < GetCapacity(list); i++) {

        if (   (GetPrevEl(list, i) == -1 && GetDataEl(list, i) != PZN)
            || (GetPrevEl(list, i) != -1 && GetDataEl(list, i) == PZN)) {

            SetErrCode(list, GetErrCode(list) | list_PZN_err);

        }

        if (   GetNextEl(list, i) >= GetCapacity(list)
            || GetNextEl(list, i) < 0) {

            SetErrCode(list, GetErrCode(list) | list_next_err);

        }

        if (   GetPrevEl(list, i) >= GetCapacity(list)
            || GetPrevEl(list, i) < -1) {

            SetErrCode(list, GetErrCode(list) | list_prev_err);

        }

        // проверка зациклинности

        if (   GetDataEl(list, i) != PZN
            && (GetNextEl(list, GetPrevEl(list, i)) != i
             || GetPrevEl(list, GetNextEl(list, i)) != i)) {

            //printf("H3 INDEX: %d\n", i);
            SetErrCode(list, GetErrCode(list) | list_loop_err);


        }
    }

    if (GetErrCode(list) & list_loop_err)
        return error;

    int count_el = 0;

    for (int i = GetHead(list); i != 0 && count_el <= GetNumOfEl(list) + 1;
                                                    i = GetNextEl(list, i))
        count_el++;

    //printf("COUNTER: %d\n", count_el);

    if (GetNumOfEl(list) != count_el) {

        SetErrCode(list, GetErrCode(list) | list_num_of_el_err);

    }

    count_el = 0;

    for (int i = GetTail(list); i != 0 && count_el <= GetNumOfEl(list) + 1;
                                                    i = GetPrevEl(list, i))
        count_el++;

    if (GetNumOfEl(list) != count_el) {

        SetErrCode(list, GetErrCode(list) | list_num_of_el_err);

    }

    if (!GetErrCode(list))
        return success;

    return error;
}

void PrintError(struct StructList* list)
{
    assert(list != NULL);

    if (GetErrCode(list) & list_loop_err)
        fprintf(log_file, "BAD_LIST_LOOP - [%d]\n", list_loop_err);

    if (GetErrCode(list) & list_capacity_err)
        fprintf(log_file, "BAD_CAPACITY - [%d]\n", list_capacity_err);

    if (GetErrCode(list) & list_head_err)
        fprintf(log_file, "BAD_HEAD - [%d]\n", list_head_err);

    if (GetErrCode(list) & list_tail_err)
        fprintf(log_file, "BAD_TAIL - [%d]\n", list_tail_err);

    if (GetErrCode(list) & list_free_err)
        fprintf(log_file, "BAD_FREE - [%d]\n", list_free_err);

    if (GetErrCode(list) & list_PZN_err)
        fprintf(log_file, "BAD_PZN - [%d]\n", list_PZN_err);

    if (GetErrCode(list) & list_next_err)
        fprintf(log_file, "BAD_NEXT - [%d]\n", list_next_err);

    if (GetErrCode(list) & list_prev_err)
        fprintf(log_file, "BAD_PREV - [%d]\n", list_loop_err);

    if (GetErrCode(list) & list_canary_err)
        fprintf(log_file, "BAD_CANARY - [%d]\n", list_canary_err);

    if (GetErrCode(list) & list_num_of_el_err)
        fprintf(log_file, "BAD_NUM_OF_ELEMENT - [%d]\n", list_num_of_el_err);
}

enum ReturnStatus ListDump(struct StructList* list,
                           const int line, const char* func, const char* file)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    if (GetErrCode(list) & list_capacity_err)
        return error;

    fprintf(log_file, "<h4>LIST { %s %s:%d }</h4>", func, file, line);

    if (GetErrCode(list) != 0)
        PrintError(list);

    static int file_counter = 0;

    char* image_file_name = GetNewImageFileName(file_counter);

    FILE* graphiz_file = fopen(image_file_name, "w");

    fprintf(graphiz_file, "digraph {\n"
                          "rankdir = HR;\n"
                          "splines = ortho\n"
                          "ranksep = 1.0\n"
                          "nodesep = 0.5\n");

    //Создание вершин

    CreateNodes(graphiz_file, list);

    //Установка ранков

    SetNodesRanks(graphiz_file, list);

    //Создание ребер

    CreateEdges(graphiz_file, list);

    fprintf(graphiz_file, "}");

    fclose(graphiz_file);
    free(image_file_name);

    char* command = GetNewDotCmd(file_counter);

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

    fprintf(log_file, "\nHEAD: %d\n", GetHead(list));
    fprintf(log_file, "TAIL: %d\n", GetTail(list));
    fprintf(log_file, "FREE: %d\n", GetFree(list));
    fprintf(log_file, "NUMBER OF ELEMENTS: %d\n\n", GetNumOfEl(list));

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

    fprintf(log_file, "-------------------------------------------------------\n\n");

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

    va_list args = {};
    va_start(args, message);

    vfprintf(log_file, message, args);
    fflush(log_file);

    va_end(args);

    ListDump(list, line, func, file);
}

void PrintBazeNode(FILE* graphiz_file, const char* name, const char* color)
{
    assert(name != NULL);
    assert(color != NULL);
    assert(graphiz_file != NULL);

    fprintf(graphiz_file, "%s "
                          "[shape = record; "
                          "style = filled; "
                          "fillcolor = \"%s\"; "
                          "color = \"#00000\"; "
                          "label = \"%s\"; ]\n",
                          name, color, name);
}

void PrintNode(FILE* graphiz_file, int index, const char* color,
                    int data, int next, int prev)
{
    assert(color != NULL);
    assert(graphiz_file != NULL);

    fprintf(graphiz_file, "node%d "
                          "[shape = record; "
                          "style = filled; "
                          "fillcolor = \"%s\"; "
                          "color = \"#00000\"; "
                          "label = \"{INDEX = %d |",
                           index, color, index);

        if (data == PZN)
            fprintf(graphiz_file, "DATA = PZN");

        else
            fprintf(graphiz_file, "DATA = %d", data);

        fprintf(graphiz_file, " | {NEXT = %d | PREV = %d} }\"; ]\n",
                                          next,       prev);
}

void CreateNodes(FILE* graphiz_file, struct StructList* list)
{
    assert(graphiz_file != NULL);
    assert(list != NULL);

    PrintBazeNode(graphiz_file, "HEAD", "#F2B138");
    PrintBazeNode(graphiz_file, "TAIL", "#B2A4F0");
    PrintBazeNode(graphiz_file, "FREE", "#77DD77");

    const char* color = NULL;

    //Создание вершин

    for (int i = 0; i < GetCapacity(list); i++) {

        if (i == 0)
            color = "#F984E5";

        else if (IsIndexFree(list, i))

            color = "#77DD77";

        else if(i == GetHead(list))

            color = "#F2B138";

        else if (i == GetTail(list))

            color = "#B2A4F0";

        else

            color = "#87CEEB";

        PrintNode(graphiz_file, i, color, GetDataEl(list, i),
                                          GetNextEl(list, i),
                                          GetPrevEl(list, i));

    }

}

void SetNodesRanks(FILE* graphiz_file, struct StructList* list)
{
    assert(graphiz_file != NULL);
    assert(list != NULL);

    fprintf(graphiz_file, "{ rank = same; ");

    for (int i = 0; i < GetCapacity(list); ++i)
        fprintf(graphiz_file, "node%d; ", i);

    fprintf(graphiz_file, "}\n");

    fprintf(graphiz_file, "{ rank = same; HEAD; TAIL; FREE}\n");

    for (int i = 0; i < GetCapacity(list) - 1; i++)
        fprintf(graphiz_file, "node%d -> node%d "
                              "[color = \"transparent\";"
                              " weight=10]\n", i, i + 1);
}

void PrintBazeEdge(FILE* graphiz_file,
                   const char* name, const char* color, int node_index)
{
    assert(name != NULL);
    assert(color != NULL);
    assert(graphiz_file != NULL);

    fprintf(graphiz_file, "%s -> node%d "
                          "[color = \"%s\","
                          " weight=10]\n", name, node_index, color);
}

void CreateEdges(FILE* graphiz_file, struct StructList* list)
{
    assert(graphiz_file != NULL);
    assert(list != NULL);

    PrintBazeEdge(graphiz_file, "HEAD", "#F2B138", GetHead(list));
    PrintBazeEdge(graphiz_file, "TAIL", "#B2A4F0", GetTail(list));
    PrintBazeEdge(graphiz_file, "FREE", "#77DD77", GetFree(list));

    for (int i = 0; i < GetCapacity(list); i++) {

        // Элемент фришный

        if (IsIndexFree(list, i)) {

            PrintEdge(graphiz_file, i, GetNextEl(list, i), "black", true);

            continue;

        }

        if (GetDataEl(list, i) == PZN && GetPrevEl(list, i) != -1)
            PrintErrorNode(graphiz_file, GetDataEl(list, i), "#FF0000", "PZN_ERR", i);


        //Если индекс валидный

        if (   IsPrevIndexCorrect(list, i)
            && IsNextIndexCorrect(list, i)) {

            if (i != GetNextEl(list, GetPrevEl(list, i)))

                PrintEdge(graphiz_file, i, GetPrevEl(list, i), "black", true);

            if (i != GetPrevEl(list, GetNextEl(list, i)))

                PrintEdge(graphiz_file, i, GetNextEl(list, i), "black", true);

            if (   i == GetPrevEl(list, GetNextEl(list, i))
                && i == GetNextEl(list, GetPrevEl(list, i)))

                PrintEdge(graphiz_file, i, GetPrevEl(list, i), "black", false);

            continue;

        }

        //Если индекс не валидный

        if (!IsNextIndexCorrect(list, i))

            PrintErrorNode(graphiz_file, GetNextEl(list, i), "#FF0000", "NEXT_ERR", i);

        if (!IsPrevIndexCorrect(list, i))

            PrintErrorNode(graphiz_file, GetPrevEl(list, i), "#FF0000", "PREV_ERR", i);

    }
}

void PrintEdge(FILE* graphiz_file,
               int name1, int name2, const char* color, bool is_oriented)
{
    assert(color != NULL);
    assert(graphiz_file != NULL);

    if (is_oriented)

        fprintf(graphiz_file, "node%d -> node%d "
                              "[color = \"%s\", "
                              " penwidth = 1.0, "
                              " weight=0]\n", name1, name2, color);

    else

        fprintf(graphiz_file, "node%d -> node%d "
                              "[color = \"%s\", "
                              " penwidth = 1.0, "
                              " dir = both, "
                              " weight=0]\n", name1, name2, color);

}

bool IsNextIndexCorrect(struct StructList* list, int index)
{
    return  GetNextEl(list, index) < GetCapacity(list) && GetNextEl(list, index) >= 0;
}

bool IsPrevIndexCorrect(struct StructList* list, int index)
{
    return  GetPrevEl(list, index) < GetCapacity(list) && GetPrevEl(list, index) >= -1;
}

bool IsIndexFree(struct StructList* list, int index)
{
    return GetPrevEl(list, index) == -1;
}

void PrintErrorNode(FILE* graphiz_file,
                    int err_index, const char* color, const char* err_name, int index)
{
    assert(color != NULL);
    assert(err_name != NULL);
    assert(graphiz_file != NULL);

    static int name = 0;

    fprintf(graphiz_file, "error%d "
                          "[shape = record; "
                          "style = filled; "
                          "fillcolor = \"%s\"; "
                          "color = \"#00000\"; "
                          "label = \"{ %s: %d }\"]\n",
                           name, color, err_name, err_index);

    fprintf(graphiz_file, "node%d -> error%d "
                          "[color = \"black\", "
                          " penwidth = 2.0, "
                          " fillcolor = \"red\", "
                          " style = \"filled\", "
                          " arrowsize = 1.5, "
                          " weight=0]\n", index, name);

    name++;
}
