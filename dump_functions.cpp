#include <TXLib.h>

#include "list_functions.h"
#include "dump_functions.h"

enum ReturnStatus ListVerifier(struct StructList* list)
{
    assert(list != NULL);

    //список пуст
    if (list->tail == list->head)
        return success;

    if (list->head >= list->capacity || list->prev[list->head] != 0) {

        list->err_code |= list_head_err;
        return error;
    }

    if (list->capacity == 0 || list->capacity > MAX_CAPACITY) {

        list->err_code |= list_capacity_err;
        return error;
    }

    if (list->tail >= list->capacity || list->next[list->tail] != 0) {

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



    for (size_t i = 1; i < list->capacity; i++) {

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

        if (i == list->head && list->prev[list->next[i]] != i) {

            //printf("H1 INDEX: %u\n", i);
            list->err_code |= list_loop_err;
            return error;
        }

        if (i == list->tail && list->next[list->prev[i]] != i) {

            //printf("H2 INDEX: %u\n", i);
            list->err_code |= list_loop_err;
            return error;
        }

        if (i != list->tail && i != list->head && list->data[i] != PZN
            && (list->next[list->prev[i]] != i || list->prev[list->next[i]] != i)) {

            //printf("H3 INDEX: %u\n", i);
            list->err_code |= list_loop_err;
            return error;

        }

    }

    return success;
}

void ListDump(struct StructList list)
{

    static int file_counter = 0;

    char* image_file_name = GetNewImageFileName(file_counter);

    FILE* graphiz_file = fopen(image_file_name, "w");

    const size_t ORIGINAL_FREE = list.free;

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

    for (size_t i = 0; i < list.capacity; i++) {

        bool is_free_index = false;

        for (size_t index_free_el = list.free;
                    index_free_el != 0;
                    index_free_el = list.next[index_free_el]) {

                 if (i == index_free_el) {
                    is_free_index = true;
                    break;
                }
        }

        if (is_free_index)

            color = "#F14065";

        else
            color = "#77DD77";

        fprintf(graphiz_file, "node%u "
                              "[shape = rectangle; "
                              "style = filled; "
                              "fillcolor = \"%s\"; "
                              "color = \"#80000\"; "
                              "label = \"INDEX = %u \\n",
                               i, color, i);

        if (list.data[i] == PZN)
            fprintf(graphiz_file, "DATA = PZN \\n");

        else
            fprintf(graphiz_file, "DATA = %d \\n", list.data[i]);

        fprintf(graphiz_file, "NEXT = %d "
                              "PREV = %d"
                              "\"; ]\n",
                              list.next[i],
                              list.prev[i]);

    }

    fprintf(graphiz_file, "{ rank = same; ");

    for (size_t i = 0; i < list.capacity; ++i) {
        fprintf(graphiz_file, "node%u; ", i);
    }

    fprintf(graphiz_file, "}\n");

    fprintf(graphiz_file, "{ rank = same; head; tail; free}\n");

    for (size_t i = 0; i < list.capacity - 1; i++) {
        fprintf(graphiz_file, "node%u -> node%u "
                              "[color = \"transparent\";"
                              " weight=100]\n", i, i + 1);
    }

    for (size_t i = list.head; i != list.tail; i = list.next[i]) {
        fprintf(graphiz_file, "node%u -> node%d "
                              "[color = \"red\","
                              " weight=0]\n", i, list.next[i]);
    }

    fprintf(graphiz_file, "node%u -> node0 "
                          "[color = \"blue\","
                          " weight=0]\n", list.tail);

    for (size_t i = list.tail; i != list.head; i = list.prev[i]) {
        fprintf(graphiz_file, "node%u -> node%d "
                              "[color = \"blue\","
                              " weight=0]\n", i, list.prev[i]);
    }

    fprintf(graphiz_file, "head -> node%u "
                          "[color = \"yellow\","
                          " weight=100]\n", list.head);

    fprintf(graphiz_file, "tail -> node%u "
                          "[color = \"yellow\","
                          " weight=100]\n", list.tail);

    fprintf(graphiz_file, "free -> node%u "
                          "[color = \"yellow\","
                          " weight=100]\n", ORIGINAL_FREE);

    fprintf(graphiz_file, "}");

    fclose(graphiz_file);
    free(image_file_name);


    char* command = GetNewDotCmd(file_counter);
    //printf("%s\n", command);
    system(command);
    free(command);

    list.free = ORIGINAL_FREE;
    FillLogFile(image_file_name, list, file_counter);

    file_counter++;
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

void FillLogFile(char* image_file_name, struct StructList list, int file_counter)
{
    assert(image_file_name != NULL);

    fprintf(log_file, "HEAD: %u\n", list.head);
    fprintf(log_file, "TAIL: %u\n", list.tail);
    fprintf(log_file, "FREE: %u\n\n", list.free);

    fprintf(log_file, "INDX: ");

    for (size_t i = 0; i < list.capacity; ++i) {
        fprintf(log_file, "|%3u| ", i);
    }

    fprintf(log_file, "\nDATA: ");

    for (size_t i = 0; i < list.capacity; ++i) {
        fprintf(log_file, "|%3d| ", list.data[i]);
    }

    fprintf(log_file, "\nNEXT: ");

    for (size_t i = 0; i < list.capacity; ++i) {
        fprintf(log_file, "|%3d| ", list.next[i]);
    }

    fprintf(log_file, "\nPREV: ");

    for (size_t i = 0; i < list.capacity; ++i) {
        fprintf(log_file, "|%3d| ", list.prev[i]);
    }

    fprintf(log_file, "\n<img src=image%d.png width=2000px>\n\n", file_counter);

    fprintf(log_file, "-------------------------------------------------\n\n");

    fflush(log_file);
}
