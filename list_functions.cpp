#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"

const char* log_file_name = "list_logs.html";

ReturnStatus ListCtor(struct StructList* list)
{
    assert(list != NULL);

    list->data = (int*)calloc(list->capacity, sizeof(int));

    list->next = (int*)calloc(list->capacity, sizeof(int));

    list->prev = (int*)calloc(list->capacity, sizeof(int));

    if (list->data == NULL || list->next == NULL || list->prev == NULL) {
        printf("Memory was not allocated");
        errno = error;
        return error;
    }

    SetDefaultNext(list);
    SetDefaultPrev(list);

    list->head = 1;
    list->tail = 1;
    list->free = 1;

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

void SetDefaultNext(struct StructList* list)
{
    assert(list != NULL);

    list->next[0] = 0;

    for (size_t i = 1; i < list->capacity; ++i)
        list->next[i] = i + 1;

    list->next[list->capacity - 1] = 0;

}

void SetDefaultPrev(struct StructList* list)
{
    assert(list != NULL);

    list->prev[0] = 0;

    for (size_t i = 1; i < list->capacity; ++i)
        list->prev[i] = -1;

}

enum ReturnStatus InsertAfter(struct StructList* list,
                              size_t prev_index, int value)
{
    assert(list != NULL);

    FILE* log_file = fopen(log_file_name, "a");
    fprintf(log_file, "До вставки после [%u] элемента %d\n", prev_index, value);
    fclose(log_file);

    ListDump(*list);



    if (list->tail == list->capacity - 1) {
        printf("List is full");
        return error;
    }

    // вставка первого элемента
    if (prev_index == 0 && list->tail == 1) {

        list->data[list->free] = value;
        list->prev[list->free] = 0;

        list->free++;
    }

    // вставка нового первого элемента

    else if (prev_index == 0) {

        list->data[list->free] = value;

        list->prev[list->free] = list->prev[list->head];
        list->prev[list->head] = list->free;

        int twin_head = list->head;
        int twin_free = list->free;

        list->head = list->free;
        list->free = list->next[list->free];
        list->next[twin_free] = twin_head;

    }

    //вставка в конец
    else if (list->tail == prev_index) {

        list->tail++;

        list->data[list->free] = value;

        list->next[prev_index] = list->free;

        list->prev[list->free] = prev_index;

        list->free = list->next[list->free];

        list->next[list->tail] = 0;

    }

    //вставка между элементами

    else {

        list->data[list->free] = value;

        int twin_free = list->free;

        list->free = list->next[list->free];

        list->next[twin_free] = list->next[prev_index];

        list->prev[list->next[prev_index]] = twin_free;

        list->next[prev_index] = twin_free;

        list->prev[twin_free] = prev_index;

    }

    log_file = fopen(log_file_name, "a");
    fprintf(log_file, "После вставки после [%u] элемента %d\n", prev_index, value);
    fclose(log_file);

    ListDump(*list);

    return success;
}

enum ReturnStatus DeleteElement(struct StructList* list, size_t del_index)
{
    assert(list != NULL);

    FILE* log_file = fopen(log_file_name, "a");
    fprintf(log_file, "До удаление [%u]", del_index);
    fclose(log_file);

    ListDump(*list);

    //список пуст
    if (list->tail == 1) {
        printf("List is empty");
        return error;
    }

    //удаление с конца
    if (del_index == list->tail) {

        list->data[del_index] = 0;

        list->tail = list->prev[del_index];

        list->next[list->prev[del_index]] = 0;
        list->next[del_index] = list->free;

        list->free = del_index;

        list->prev[del_index] = -1;
    }

    //удаление первого элемента
    else if (del_index == list->head) {

       list->data[del_index] = 0;

       list->head = list->next[del_index];

       list->next[del_index] = list->free;
       list->free = del_index;

       list->prev[del_index] = -1;

    }

    //удаление из середины
    else {
        list->data[del_index] = 0;
        list->next[list->prev[del_index]] = list->next[del_index];

        list->prev[list->next[del_index]] = list->prev[del_index];

        list->prev[del_index] = -1;

        list->next[del_index] = list->free;
        list->free = del_index;
    }

    log_file = fopen(log_file_name, "a");
    fprintf(log_file, "После удаление [%u]", del_index);
    fclose(log_file);

    ListDump(*list);

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

        if (is_free_index){

            color = "#F14065";
        }
        else
            color = "#77DD77";

        fprintf(graphiz_file, "node%u "
                              "[shape = rectangle; "
                              "style = filled; "
                              "fillcolor = \"%s\"; "
                              "color = \"#80000\"; "
                              "label = \"INDEX = %u \\n"
                                         "DATA = %d  \\n"
                                         "NEXT = %d "
                                         "PREV = %d"
                                         "\"; ]\n",
                                         i, color, i,
                                         list.data[i],
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

    FILE* log_file = fopen(log_file_name, "a");

    fprintf(log_file, "<pre>\n");
    fprintf(log_file, "<h3> DUMP </h3>\n");

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

    fclose(log_file);
}

void UserPrintList(struct StructList list)
{
    for (int i = list.head; i != 0; i = list.next[i]) {
        printf("%d ", list.data[i]);
    }
}






