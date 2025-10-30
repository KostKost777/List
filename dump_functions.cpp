#include <TXLib.h>

#include "list_functions.h"
#include "dump_functions.h"

enum ReturnStatus ListVerifier(struct StructList* list)
{
    assert(list != NULL);

    //список пуст
    if (list->prev[0] == list->next[0])
        return success;

    if (list->capacity == 0 || list->capacity > MAX_CAPACITY) {
        list->err_code |= list_capacity_err;
        return error;
    }

    if (list->next[0] >= list->capacity || list->prev[list->next[0]] != 0) {

        list->err_code |= list_head_err;
        return error;
    }

    if (list->prev[0] >= list->capacity || list->next[list->prev[0]] != 0) {

        list->err_code |= list_tail_err;
        return error;
    }

    if (list->free == 0) {

        list->err_code |= list_size_err;
        return error;
    }

    if (   list->free >= list->capacity
        || list->data[list->free] != PZN
        || list->prev[list->free] != -1) {

        list->err_code |= list_free_err;
        return error;
    }

    for (int i = 1; i < list->capacity; i++) {

        if (   (list->prev[i] == -1 && list->data[i] != PZN)
            || (list->prev[i] != -1 && list->data[i] == PZN)) {

            list->err_code |= list_PZN_err;
            return error;

        }

        if (list->next[i] >= list->capacity) {

            list->err_code |= list_next_err;
            return error;
        }

        if (  (list->prev[i] >= 0 && list->prev[i] >= list->capacity)
            || list->prev[i] < -1) {

            list->err_code |= list_prev_err;
            return error;
        }

        // проверка зациклинности

        if (i == list->next[0] && list->prev[list->next[i]] != i) {

            //printf("H1 INDEX: %d\n", i);
            list->err_code |= list_loop_err;
            return error;
        }

        if (i == list->prev[0] && list->next[list->prev[i]] != i) {

            //printf("H2 INDEX: %d\n", i);
            list->err_code |= list_loop_err;
            return error;
        }

        if (i != list->prev[0] && i != list->next[0] && list->data[i] != PZN
            && (list->next[list->prev[i]] != i || list->prev[list->next[i]] != i)) {

            //printf("H3 INDEX: %d\n", i);
            list->err_code |= list_loop_err;
            return error;

        }

    }

    return success;
}

void PrintError(struct StructList* list)
{
    assert(list != NULL);

    if (list->err_code & list_loop_err)
        fprintf(log_file, "<h2>BAD_LIST_LOOP - [%d]</h2>\n", list_loop_err);

    if (list->err_code & list_capacity_err)
        fprintf(log_file, "<h2>BAD_CAPACITY - [%d]</h2>\n", list_capacity_err);

    if (list->err_code & list_head_err)
        fprintf(log_file, "<h2>BAD_HEAD - [%d]</h2>\n", list_head_err);

    if (list->err_code & list_tail_err)
        fprintf(log_file, "<h2>BAD_TAIL - [%d]</h2>\n", list_tail_err);

    if (list->err_code & list_free_err)
        fprintf(log_file, "<h2>BAD_FREE - [%d]</h2>\n", list_free_err);

    if (list->err_code & list_size_err)
        fprintf(log_file, "<h2>BAD_SIZE - [%d]</h2>\n", list_size_err);

    if (list->err_code & list_PZN_err)
        fprintf(log_file, "<h2>BAD_PZN - [%d]</h2>\n", list_PZN_err);

    if (list->err_code & list_next_err)
        fprintf(log_file, "<h2>BAD_NEXT - [%d]</h2>\n", list_next_err);

    if (list->err_code & list_prev_err)
        fprintf(log_file, "<h2>BAD_PREV - [%d]</h2>\n", list_loop_err);

}

enum ReturnStatus ListDump(struct StructList* list,
                           const int line, const char* func, const char* file)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    if (list->err_code != 0) {
        PrintError(list);
        return fatal_error;
    }

    fprintf(log_file, "<h4>LIST { %s %s:%d }</h4>\n", file, func, line);

    static int file_counter = 0;

    char* image_file_name = GetNewImageFileName(file_counter);

    FILE* graphiz_file = fopen(image_file_name, "w");

    const int ORIGINAL_FREE = list->free;

    fprintf(graphiz_file, "digraph {\n"
                          "rankdir = HR;\n"
                          "splines = ortho\n");

    fprintf(graphiz_file, "head "
                          "[shape = rectangle; "
                          "style = filled; "
                          "fillcolor = \"#ECF96A\"; "
                          "color = \"#80000\"; "
                          "label = \"HEAD\"; ]\n");

    fprintf(graphiz_file, "tail "
                          "[shape = rectangle; "
                          "style = filled; "
                          "fillcolor = \"#ECF96A\"; "
                          "color = \"#80000\"; "
                          "label = \"TAIL\"; ]\n");

    fprintf(graphiz_file, "free "
                          "[shape = rectangle; "
                          "style = filled; "
                          "fillcolor = \"#ECF96A\"; "
                          "color = \"#80000\"; "
                          "label = \"FREE\"; ]\n");

    const char* color = NULL;

    for (int i = 0; i < list->capacity; i++) {

        bool is_free_index = false;

        for (int index_free_el = list->free;
                    index_free_el != 0;
                    index_free_el = list->next[index_free_el]) {

                 if (i == index_free_el) {
                    is_free_index = true;
                    break;
                }
        }

        if (is_free_index)

            color = "#F14065";

        else
            color = "#77DD77";

        fprintf(graphiz_file, "node%d "
                              "[shape = rectangle; "
                              "style = filled; "
                              "fillcolor = \"%s\"; "
                              "color = \"#80000\"; "
                              "label = \"INDEX = %d \\n",
                               i, color, i);

        if (list->data[i] == PZN)
            fprintf(graphiz_file, "DATA = PZN \\n");

        else
            fprintf(graphiz_file, "DATA = %d \\n", list->data[i]);

        fprintf(graphiz_file, "NEXT = %d "
                              "PREV = %d"
                              "\"; ]\n",
                              list->next[i],
                              list->prev[i]);

    }

    fprintf(graphiz_file, "{ rank = same; ");

    for (int i = 0; i < list->capacity; ++i) {
        fprintf(graphiz_file, "node%d; ", i);
    }

    fprintf(graphiz_file, "}\n");

    fprintf(graphiz_file, "{ rank = same; head; tail; free}\n");

    for (int i = 0; i < list->capacity - 1; i++) {
        fprintf(graphiz_file, "node%d -> node%d "
                              "[color = \"transparent\";"
                              " weight=100]\n", i, i + 1);
    }

    for (int i = list->next[0]; i != list->prev[0]; i = list->next[i]) {
        fprintf(graphiz_file, "node%d -> node%d "
                              "[color = \"red\","
                              " weight=0]\n", i, list->next[i]);
    }

    fprintf(graphiz_file, "node%d -> node0 "
                          "[color = \"blue\","
                          " weight=0]\n", list->prev[0]);

    for (int i = list->prev[0]; i != list->next[0]; i = list->prev[i]) {
        fprintf(graphiz_file, "node%d -> node%d "
                              "[color = \"blue\","
                              " weight=0]\n", i, list->prev[i]);
    }

    fprintf(graphiz_file, "head -> node%d "
                          "[color = \"yellow\","
                          " weight=100]\n", list->next[0]);

    fprintf(graphiz_file, "tail -> node%d "
                          "[color = \"yellow\","
                          " weight=100]\n", list->prev[0]);

    fprintf(graphiz_file, "free -> node%d "
                          "[color = \"yellow\","
                          " weight=100]\n", ORIGINAL_FREE);

    fprintf(graphiz_file, "}");

    fclose(graphiz_file);
    free(image_file_name);


    char* command = GetNewDotCmd(file_counter);
    //printf("%s\n", command);
    system(command);
    free(command);

    list->free = ORIGINAL_FREE;
    FillLogFile(image_file_name, list, file_counter);

    file_counter++;

    return success;
}

char* GetNewDotCmd(int file_counter)
{
    char str_command[100] = "";

    snprintf(str_command, sizeof(str_command),
            "dot -Tpng image%d.txt -o image%d.png",
             file_counter, file_counter);

    return strdup(str_command);
}

char* GetNewImageFileName(int file_counter)
{
    char str_file_counter[100] = "";

    snprintf(str_file_counter, sizeof(str_file_counter),
             "image%d.txt", file_counter);

    return strdup(str_file_counter);
}

void FillLogFile(char* image_file_name, struct StructList* list, int file_counter)
{
    assert(image_file_name != NULL);
    assert(list != NULL);

    fprintf(log_file, "HEAD: %d\n", list->next[0]);
    fprintf(log_file, "TAIL: %d\n", list->prev[0]);
    fprintf(log_file, "FREE: %d\n\n", list->free);

    fprintf(log_file, "INDX: ");

    for (int i = 0; i < list->capacity; ++i) {
        fprintf(log_file, "|%3d| ", i);
    }

    fprintf(log_file, "\nDATA: ");

    for (int i = 0; i < list->capacity; ++i) {
        fprintf(log_file, "|%3d| ", list->data[i]);
    }

    fprintf(log_file, "\nNEXT: ");

    for (int i = 0; i < list->capacity; ++i) {
        fprintf(log_file, "|%3d| ", list->next[i]);
    }

    fprintf(log_file, "\nPREV: ");

    for (int i = 0; i < list->capacity; ++i) {
        fprintf(log_file, "|%3d| ", list->prev[i]);
    }

    fprintf(log_file, "\n\n<img src=image%d.png width=2000px>\n\n", file_counter);

    fprintf(log_file, "-------------------------------------------------------\n");

    fflush(log_file);
}
